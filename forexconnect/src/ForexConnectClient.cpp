#include "ForexConnectClient.h"
#include "TableHandler.h"
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/bind.hpp>
#include <string.h>
#include <stdexcept>
using namespace pyforexconnect;

namespace
{
    template <class T>
    boost::python::list vector_to_python_list(const std::vector<T>& vector) {
	typename std::vector<T>::const_iterator iter;
	boost::python::list list;
	for (iter = vector.begin(); iter != vector.end(); ++iter) {
	    list.append(*iter);
	}
	return list;
    }

    template <class K, class V>
    boost::python::dict map_to_python_dict(const std::map<K, V>& map) {
	typename std::map<K, V>::const_iterator iter;
	boost::python::dict dictionary;
	for (iter = map.begin(); iter != map.end(); ++iter) {
	    dictionary[iter->first] = iter->second;
	}
	return dictionary;
    }
}

LoginParams::LoginParams()
{
}

LoginParams::LoginParams(const std::string& login,
			 const std::string& password,
			 const std::string& connection,
			 const std::string& url)
    : mLogin(login),
      mPassword(password),
      mConnection(connection),
      mUrl(url)
{
}

std::ostream& pyforexconnect::operator<<(std::ostream& out, LoginParams const& lp)
{
    out << "<'login': " << lp.mLogin
	<< ", 'password': " << lp.mPassword
	<< ", 'connection': " << lp.mConnection
	<< ", 'url': " << lp.mUrl << ">";
    return out;
}

AccountInfo::AccountInfo()
    : mBalance(0.0),
      mUsedMargin(0.0),
      mUsableMargin(0.0),
      mBaseUnitSize(0),
      mEquity(0.0),
      mGrossPL(0.0)
{
}

std::ostream& pyforexconnect::operator<<(std::ostream& out, AccountInfo const& ai)
{
    out << "<'balance': " << ai.mBalance
	<< ", 'used_margin': " << ai.mUsedMargin
	<< ", 'usable_margin': " << ai.mUsableMargin
	<< ", 'base_unit_size': " << ai.mBaseUnitSize
	<< ", 'equity': " << ai.mEquity
	<< ", 'gross_pl': " << ai.mGrossPL;
}

TradeInfo::TradeInfo()
    : mOpenRate(0.0),
      mOpenDate(boost::posix_time::second_clock::local_time()),
      mGrossPL(0.0)
{
}

bool TradeInfo::operator==(const TradeInfo& other)
{
    return mTradeID == other.mTradeID;
}

bool TradeInfo::operator!=(const TradeInfo& other)
{
    return mTradeID != other.mTradeID;
}

std::ostream& pyforexconnect::operator<<(std::ostream& out, TradeInfo const& ti)
{
    out << "<'instrument': " << ti.mInstrument
	<< ", 'trade_id': " << ti.mTradeID
	<< ", 'but_sell': " << ti.mBuySell
	<< ", 'open_rate': " << ti.mOpenRate
	<< ", 'amount': " << ti.mAmount
	<< ", 'open_date': " << ti.mOpenDate
	<< ", 'gross_pl': " << ti.mGrossPL << ">";
    return out;
}

Prices::Prices()
    : mDate(boost::posix_time::second_clock::local_time()),
      mOpen(0.0),
      mHigh(0.0),
      mLow(0.0),
      mClose(0.0)
{
}

Prices::Prices(boost::posix_time::ptime date,
	       double value)
    : mDate(date),
      mOpen(value),
      mHigh(value),
      mLow(value),
      mClose(value)
{
}

Prices::Prices(boost::posix_time::ptime date,
	       double open,
	       double high,
	       double low,
	       double close)
    : mDate(date),
      mOpen(open),
      mHigh(high),
      mLow(low),
      mClose(close)
{
}

bool Prices::operator==(const Prices& other)
{
    return mDate == other.mDate &&
	mOpen == other.mOpen &&
	mHigh == other.mHigh &&
	mLow == other.mLow &&
	mClose == other.mClose;
}

bool Prices::operator!=(const Prices& other)
{
    return !(*this == other);
}

std::ostream& pyforexconnect::operator<<(std::ostream& out, Prices const& pr)
{
    out << "<'date': " << pr.mDate
	<< ", 'open': " << pr.mOpen
	<< ", 'high': " << pr.mHigh
	<< ", 'low': " << pr.mLow
	<< ", 'close': " << pr.mClose << ">";
    return out;
}

ForexConnectClient::ForexConnectClient(const std::string& login,
				       const std::string& password,
				       const std::string& connection,
				       const std::string& url)
    : mLoginParams(login, password, connection, url),
      mpSession(NULL),
      mpListener(NULL),
      mpResponseListener(NULL),
      mpLoginRules(NULL),
      mpAccountRow(NULL),
      mpResponseReaderFactory(NULL),
      mpRequestFactory(NULL)
{
    init();
}

ForexConnectClient::ForexConnectClient(const LoginParams& loginParams)
    : mLoginParams(loginParams),
      mpSession(NULL),
      mpListener(NULL),
      mpResponseListener(NULL),
      mpLoginRules(NULL),
      mpAccountRow(NULL),
      mpResponseReaderFactory(NULL),
      mpRequestFactory(NULL)
{
    init();
}

ForexConnectClient::~ForexConnectClient()
{
    mpRequestFactory->release();
    mpAccountRow->release();
    mpLoginRules->release();
    mpResponseReaderFactory->release();
    mpSession->unsubscribeResponse(mpResponseListener);
    mpResponseListener->release();
    if (mIsConnected)
    {
	logout();
    }
    mpSession->unsubscribeSessionStatus(mpListener);
    mpListener->release();
    mpSession->release();
}

void ForexConnectClient::init()
{
    mpSession = CO2GTransport::createSession();
    mpListener = new SessionStatusListener(mpSession, false);
    mpSession->subscribeSessionStatus(mpListener);
    mpSession->useTableManager(Yes, 0);

    if (!login())
    {
	throw std::runtime_error("Login fail.");
    }

    mpLoginRules = mpSession->getLoginRules();
    if (!mpLoginRules->isTableLoadedByDefault(Accounts))
    {
	logout();
        throw std::runtime_error("Accounts table not loaded");
    }

    O2G2Ptr<IO2GResponse> response = mpLoginRules->getTableRefreshResponse(Accounts);
    if(!response)
    {
	logout();
        throw std::runtime_error("No response to refresh accounts table request");
    }

    mpResponseReaderFactory = mpSession->getResponseReaderFactory();
    O2G2Ptr<IO2GAccountsTableResponseReader> accountsResponseReader = mpResponseReaderFactory->createAccountsTableReader(response);
    mpAccountRow = accountsResponseReader->getRow(0);
    mAccountID = mpAccountRow->getAccountID();

    mpResponseListener = new ResponseListener(mpSession);
    mpSession->subscribeResponse(mpResponseListener);

    mpRequestFactory = mpSession->getRequestFactory();
}

bool ForexConnectClient::login()
{
    mpListener->reset();
    BOOST_LOG_TRIVIAL(debug) << "user name: " << mLoginParams.mLogin;
    BOOST_LOG_TRIVIAL(debug) << "password: " << mLoginParams.mPassword;
    BOOST_LOG_TRIVIAL(debug) << "url: " << mLoginParams.mUrl;
    BOOST_LOG_TRIVIAL(debug) << "connection: " << mLoginParams.mConnection;
    mpSession->login(mLoginParams.mLogin.c_str(),
		     mLoginParams.mPassword.c_str(),
		     mLoginParams.mUrl.c_str(),
		     mLoginParams.mConnection.c_str());
    mIsConnected = mpListener->waitEvents() && mpListener->isConnected();
    return mIsConnected;
}

void ForexConnectClient::logout()
{
    mpListener->reset();
    mpSession->logout();
    mpListener->waitEvents();
    mIsConnected = false;
}

std::string ForexConnectClient::getAccountID() const
{
    return mpAccountRow->getAccountID();
}

AccountInfo ForexConnectClient::getAccountInfo()
{
    TableHandler<Accounts, IO2GAccountsTable, IO2GAccountTableRow> handler(mpSession);
    while (true)
    {
	IO2GAccountTableRow *account = handler.getNextRow();
	if (account == NULL)
	{
	    throw std::runtime_error("Could not get account table row.");
	}
        if (mAccountID.size() == 0 || strcmp(account->getAccountID(), mAccountID.c_str()) == 0)
	{
            if (strcmp(account->getMarginCallFlag(), "N") == 0 &&
                (strcmp(account->getAccountKind(), "32") == 0 ||
		 strcmp(account->getAccountKind(), "36") == 0))
	    {
		AccountInfo info;
		info.mBalance = account->getBalance();
		info.mUsedMargin = account->getUsedMargin();
		info.mUsableMargin = account->getUsableMargin();
		info.mBaseUnitSize = account->getBaseUnitSize();
		info.mEquity = account->getEquity();
		info.mGrossPL = account->getGrossPL();
                return info;
	    }
	}
    }
}

std::map<std::string, std::string> ForexConnectClient::getOffers()
{
    std::map<std::string, std::string> offers;
    TableHandler<Offers, IO2GOffersTable, IO2GOfferTableRow> handler(mpSession);
    while (handler.getNextRow())
    {
	IO2GOfferTableRow *offerRow = handler.getRow();
        offers[offerRow->getInstrument()] = offerRow->getOfferID();
    }
    return offers;
}

boost::python::dict ForexConnectClient::getOffersForPython()
{
    return map_to_python_dict(getOffers());
}

bool ForexConnectClient::isConnected() const
{
    return mIsConnected;
}

std::vector<TradeInfo> ForexConnectClient::getTrades()
{
    std::vector<TradeInfo> trades;
    TableHandler<Trades, IO2GTradesTable, IO2GTradeTableRow> handler(mpSession);
    std::map<std::string, std::string> offers = getOffers();
    while (handler.getNextRow())
    {
	IO2GTradeTableRow* tradeRow = handler.getRow();
	TradeInfo trade;
	const std::map<std::string, std::string>::const_iterator it = std::find_if(offers.begin(),
										   offers.end(),
										   boost::bind(&std::map<std::string, std::string>::value_type::second, _1) == tradeRow->getOfferID());
	if (it == offers.end())
	{
	    throw std::runtime_error("Could not get offer table row.");
	}
        trade.mInstrument = it->first;
	trade.mTradeID = tradeRow->getTradeID();
        trade.mBuySell = tradeRow->getBuySell();
        trade.mOpenRate = tradeRow->getOpenRate();
        trade.mAmount = tradeRow->getAmount();
        trade.mOpenDate = toPtime(tradeRow->getOpenTime());
        trade.mGrossPL = tradeRow->getGrossPL();
	trades.push_back(trade);
    }
    return trades;
}

boost::python::list ForexConnectClient::getTradesForPython()
{
    return vector_to_python_list(getTrades());
}

bool ForexConnectClient::openPosition(const std::string& instrument,
				      const std::string& buysell,
				      int amount)
{
    if (buysell != O2G2::Sell && buysell != O2G2::Buy)
    {
	return false;
    }

    std::map<std::string, std::string> offers = getOffers();
    std::string offerID;
    std::map<std::string, std::string>::const_iterator offer_itr = offers.find(instrument);
    if (offer_itr != offers.end()) {
	offerID = offer_itr->second;
    } else {
	BOOST_LOG_TRIVIAL(error) << "Could not find offer row for instrument " << instrument;
	return false;
    }
    O2G2Ptr<IO2GTradingSettingsProvider> tradingSettingsProvider = mpLoginRules->getTradingSettingsProvider();
    int iBaseUnitSize = tradingSettingsProvider->getBaseUnitSize(instrument.c_str(), mpAccountRow);
    O2G2Ptr<IO2GValueMap> valuemap = mpRequestFactory->createValueMap();
    valuemap->setString(Command, O2G2::Commands::CreateOrder);
    valuemap->setString(OrderType, O2G2::Orders::TrueMarketOpen);
    valuemap->setString(AccountID, mAccountID.c_str());
    valuemap->setString(OfferID, offerID.c_str());
    valuemap->setString(BuySell, buysell.c_str());
    valuemap->setInt(Amount, amount * iBaseUnitSize);
    valuemap->setString(TimeInForce, O2G2::TIF::IOC);
    valuemap->setString(CustomID, "TrueMarketOrder");
    O2G2Ptr<IO2GRequest> request = mpRequestFactory->createOrderRequest(valuemap);
    if (!request)
    {
        BOOST_LOG_TRIVIAL(error) << mpRequestFactory->getLastError();
        return false;
    }
    mpResponseListener->setRequestID(request->getRequestID());
    mpSession->sendRequest(request);
    if (mpResponseListener->waitEvents())
    {
	Sleep(1000); // Wait for the balance update
	BOOST_LOG_TRIVIAL(info) << "Done!";
	return true;
    }
    BOOST_LOG_TRIVIAL(error) << "Response waiting timeout expired";
    return false;
}

bool ForexConnectClient::closePosition(const std::string& tradeID)
{
    TableHandler<Trades, IO2GTradesTable, IO2GTradeTableRow> handler(mpSession);
    IO2GTradeTableRow *tradeRow = NULL;
    IO2GTableIterator tableIterator;
    while (true) {
	tradeRow = handler.getNextRow();
	if (!tradeRow) {
	    BOOST_LOG_TRIVIAL(error) << "Could not find trade with ID = " << tradeID;
	    return false;
	}
	if (tradeID == tradeRow->getTradeID()) {
	    break;
	}
    }
    O2G2Ptr<IO2GValueMap> valuemap = mpRequestFactory->createValueMap();
    valuemap->setString(Command, O2G2::Commands::CreateOrder);
    valuemap->setString(OrderType, O2G2::Orders::TrueMarketClose);
    valuemap->setString(AccountID, mAccountID.c_str());
    valuemap->setString(OfferID, tradeRow->getOfferID());
    valuemap->setString(TradeID, tradeID.c_str());
    valuemap->setString(BuySell, (strcmp(tradeRow->getBuySell(), O2G2::Buy) == 0) ? O2G2::Sell : O2G2::Buy);
    valuemap->setInt(Amount, tradeRow->getAmount());
    valuemap->setString(CustomID, "CloseMarketOrder");
    O2G2Ptr<IO2GRequest> request = mpRequestFactory->createOrderRequest(valuemap);
    if (!request)
    {
        BOOST_LOG_TRIVIAL(error) << mpRequestFactory->getLastError();
        return false;
    }
    mpResponseListener->setRequestID(request->getRequestID());
    mpSession->sendRequest(request);
    if (mpResponseListener->waitEvents())
    {
	Sleep(1000); // Wait for the balance update
	BOOST_LOG_TRIVIAL(info) << "Done!";
	return true;
    }
    BOOST_LOG_TRIVIAL(error) << "Response waiting timeout expired";
    return false;
}

double ForexConnectClient::getBid(const std::string& instrument) {
    TableHandler<Offers, IO2GOffersTable, IO2GOfferTableRow> handler(mpSession);
    while (handler.getNextRow())
    {
	IO2GOfferTableRow* offerRow = handler.getRow();
        if (offerRow->getInstrument() == instrument)
	{
	    return offerRow->getBid();
	}
    }
    throw std::runtime_error("Could not get offer table row.");
}

double ForexConnectClient::getAsk(const std::string& instrument) {
    TableHandler<Offers, IO2GOffersTable, IO2GOfferTableRow> handler(mpSession);
    while (handler.getNextRow())
    {
	IO2GOfferTableRow* offerRow = handler.getRow();
        if (offerRow->getInstrument() == instrument)
	{
	    return offerRow->getAsk();
	}
    }
    throw std::runtime_error("Could not get offer table row.");
}

std::vector<Prices> ForexConnectClient::getHistoricalPrices(const std::string& instrument,
							    const boost::posix_time::ptime& from,
							    const boost::posix_time::ptime& to,
    							    const std::string& timeFrame)
{
    std::vector<Prices> prices;
    O2G2Ptr<IO2GTimeframeCollection> timeframeCollection = mpRequestFactory->getTimeFrameCollection();
    O2G2Ptr<IO2GTimeframe> timeframe = timeframeCollection->get(timeFrame.c_str());
    if (!timeframe)
    {
        BOOST_LOG_TRIVIAL(error) << "Timeframe '" << timeFrame << "' is incorrect!";
        return prices;
    }
    O2G2Ptr<IO2GRequest> request = mpRequestFactory->createMarketDataSnapshotRequestInstrument(instrument.c_str(),
											       timeframe,
											       timeframe->getQueryDepth());
    DATE dtFrom = toOleTime(from);
    DATE dtTo = toOleTime(to);
    DATE dtFirst = dtTo;
    do
    {
        mpRequestFactory->fillMarketDataSnapshotRequestTime(request, dtFrom, dtFirst, false);
        mpResponseListener->setRequestID(request->getRequestID());
        mpSession->sendRequest(request);
        if (!mpResponseListener->waitEvents())
        {
            BOOST_LOG_TRIVIAL(error) << "Response waiting timeout expired";
            return prices;
        }
        // shift "to" bound to oldest datetime of returned data
        O2G2Ptr<IO2GResponse> response = mpResponseListener->getResponse();
        if (response && response->getType() == MarketDataSnapshot)
        {
	    O2G2Ptr<IO2GMarketDataSnapshotResponseReader> reader = mpResponseReaderFactory->createMarketDataSnapshotReader(response);
	    if (reader->size() > 0)
	    {
		if (fabs(dtFirst - reader->getDate(0)) > 0.0001)
		    dtFirst = reader->getDate(0); // earliest datetime of returned data
		else
		    break;
	    }
	    else
	    {
		BOOST_LOG_TRIVIAL(warning) << "0 rows received";
		break;
	    }
	    std::vector<Prices> prc = getPricesFromResponse(response);
	    prices.insert(prices.end(), prc.begin(), prc.end());
	}
	else
	{
	    break;
	}
    } while (dtFirst - dtFrom > 0.0001);
    return prices;
}

boost::python::list ForexConnectClient::getHistoricalPricesForPython(const std::string& instrument,
								     const boost::posix_time::ptime& from,
								     const boost::posix_time::ptime& to,
								     const std::string& timeFrame)
{
    return vector_to_python_list(getHistoricalPrices(instrument,
						     from,
						     to,
						     timeFrame));
}

std::vector<Prices> ForexConnectClient::getPricesFromResponse(IO2GResponse* response)
{
    std::vector<Prices> prices;
    if (!response || response->getType() != MarketDataSnapshot)
    {
	return prices;
    }
    BOOST_LOG_TRIVIAL(debug) << "Request with RequestID='" << response->getRequestID() << "' is completed:";
    O2G2Ptr<IO2GMarketDataSnapshotResponseReader> reader = mpResponseReaderFactory->createMarketDataSnapshotReader(response);
    if (!reader)
    {
	return prices;
    }
    for (int ii = reader->size() - 1; ii >= 0; ii--)
    {
	DATE dt = reader->getDate(ii);
	if (reader->isBar())
	{
	    prices.push_back(Prices(toPtime(dt),
				    reader->getAskOpen(ii),
				    reader->getAskHigh(ii),
				    reader->getAskLow(ii),
				    reader->getAskClose(ii)));
	}
	else
	{
	    prices.push_back(Prices(toPtime(dt),
				    reader->getAsk(ii)));
	}
    }
    return prices;
}

void pyforexconnect::setLogLevel(int level)
{
    boost::log::core::get()->set_filter(boost::log::trivial::severity >= static_cast<boost::log::trivial::severity_level>(level));
}
