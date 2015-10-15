#include "ForexConnectClient.h"
#include <boost/log/trivial.hpp>
#include <string.h>
#include <stdexcept>
using namespace pyforexconnect;

static std::string FOREX_URL = "http://www40.fxcorporate.com/Hosts.jsp";

namespace
{
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
			 const std::string& connection)
    : mLogin(login),
      mPassword(password),
      mConnection(connection)
{
}

std::ostream& pyforexconnect::operator<<(std::ostream& out, LoginParams const& lp)
{
    out << "login: " << lp.mLogin << std::endl
	<< "password: " << lp.mPassword << std::endl
	<< "connection: " << lp.mConnection;
    return out;
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
    out << "instrument: " << ti.mInstrument << std::endl
	<< "trade_id: " << ti.mTradeID << std::endl
	<< "but_sell: " << ti.mBuySell << std::endl
	<< "open_rate: " << ti.mOpenRate << std::endl
	<< "amount: " << ti.mAmount << std::endl
	<< "open_date: " << ti.mOpenDate << std::endl
	<< "gross_pl: " << ti.mGrossPL;
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
    out << "date: " << pr.mDate << std::endl
	<< "open: " << pr.mOpen << std::endl
	<< "high: " << pr.mHigh << std::endl
	<< "low: " << pr.mLow << std::endl
	<< "close: " << pr.mClose << std::endl;
    return out;
}

SessionStatusListener::SessionStatusListener(IO2GSession *session,
					     bool printSubsessions,
					     const std::string& sessionID,
					     const std::string& pin)
    : mSessionID(sessionID),
      mPin(pin),
      mSession(session)
{
    mSession->addRef();
    reset();
    mPrintSubsessions = printSubsessions;
    mRefCount = 1;
    mSessionEvent = CreateEvent(0, FALSE, FALSE, 0);
}

SessionStatusListener::~SessionStatusListener()
{
    mSession->release();
    mSessionID.clear();
    mPin.clear();
    CloseHandle(mSessionEvent);
}

long SessionStatusListener::addRef()
{
    return InterlockedIncrement(&mRefCount);
}

long SessionStatusListener::release()
{
    long rc = InterlockedDecrement(&mRefCount);
    if (rc == 0)
        delete this;
    return rc;
}

void SessionStatusListener::reset()
{
    mConnected = false;
    mDisconnected = false;
    mError = false;
}

void SessionStatusListener::onLoginFailed(const char *error)
{
    BOOST_LOG_TRIVIAL(error) << "Login error: " << error;
    mError = true;
}

void SessionStatusListener::onSessionStatusChanged(IO2GSessionStatus::O2GSessionStatus status)
{
    switch (status)
    {
    case IO2GSessionStatus::Disconnected:
        BOOST_LOG_TRIVIAL(info) << "status::disconnected";
        mConnected = false;
        mDisconnected = true;
        SetEvent(mSessionEvent);
        break;
    case IO2GSessionStatus::Connecting:
        BOOST_LOG_TRIVIAL(info) << "status::connecting";
        break;
    case IO2GSessionStatus::TradingSessionRequested:
    {
        BOOST_LOG_TRIVIAL(info) << "status::trading session requested";
        O2G2Ptr<IO2GSessionDescriptorCollection> descriptors = mSession->getTradingSessionDescriptors();
        bool found = false;
        if (descriptors)
        {
            if (mPrintSubsessions)
                BOOST_LOG_TRIVIAL(info) << "descriptors available:";
            for (int i = 0; i < descriptors->size(); ++i)
            {
                O2G2Ptr<IO2GSessionDescriptor> descriptor = descriptors->get(i);
                if (mPrintSubsessions)
                    BOOST_LOG_TRIVIAL(info) << "  id:='" << descriptor->getID()
					    << "' name='" << descriptor->getName()
					    << "' description='" << descriptor->getDescription()
					    << "' " << (descriptor->requiresPin() ? "requires pin" : "");
                if (mSessionID == descriptor->getID())
                {
                    found = true;
                    break;
                }
            }
        }
        if (!found)
        {
            onLoginFailed("The specified sub session identifier is not found");
        }
        else
        {
            mSession->setTradingSession(mSessionID.c_str(), mPin.c_str());
        }
    }
    break;
    case IO2GSessionStatus::Connected:
        BOOST_LOG_TRIVIAL(info) << "status::connected";
        mConnected = true;
        mDisconnected = false;
        SetEvent(mSessionEvent);
        break;
    case IO2GSessionStatus::Reconnecting:
        BOOST_LOG_TRIVIAL(info) << "status::reconnecting";
        break;
    case IO2GSessionStatus::Disconnecting:
        BOOST_LOG_TRIVIAL(info) << "status::disconnecting";
        break;
    case IO2GSessionStatus::SessionLost:
        BOOST_LOG_TRIVIAL(info) << "status::session lost";
        break;
    }
}

bool SessionStatusListener::hasError() const
{
    return mError;
}

bool SessionStatusListener::isConnected() const
{
    return mConnected;
}

bool SessionStatusListener::isDisconnected() const
{
    return mDisconnected;
}

bool SessionStatusListener::waitEvents()
{
    return WaitForSingleObject(mSessionEvent, _TIMEOUT) == 0;
}

ForexConnectClient::ForexConnectClient(const std::string& login,
				       const std::string& password,
				       const std::string& connection)
    : mLoginParams(login, password, connection),
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
    mpSession->login(mLoginParams.mLogin.c_str(),
		     mLoginParams.mPassword.c_str(),
		     FOREX_URL.c_str(),
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

double ForexConnectClient::getUsedMargin() const
{
    return mpAccountRow->getUsedMargin();
}

double ForexConnectClient::getBalance() const
{
    return mpAccountRow->getBalance();
}

std::map<std::string, std::string> ForexConnectClient::getOffers()
{
    std::map<std::string, std::string> offers;
    O2G2Ptr<IO2GTableManager> tableManager = getLoadedTableManager();
    O2G2Ptr<IO2GOffersTable> offersTable = static_cast<IO2GOffersTable*>(tableManager->getTable(Offers));
    IO2GOfferTableRow *offerRow = NULL;
    IO2GTableIterator iterator;
    while (offersTable->getNextRow(iterator, offerRow))
    {
        offers[offerRow->getInstrument()] = offerRow->getOfferID();
        offerRow->release();
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
    O2G2Ptr<IO2GTableManager> tableManager = getLoadedTableManager();
    O2G2Ptr<IO2GTradesTable> tradesTable = static_cast<IO2GTradesTable*>(tableManager->getTable(Trades));
    IO2GTradeTableRow* tradeRow = NULL;
    IO2GTableIterator tableIterator;
    while (tradesTable->getNextRow(tableIterator, tradeRow))
    {
        O2G2Ptr<IO2GOfferRow> offer = getTableRow<IO2GOfferRow, IO2GOffersTableResponseReader>(Offers, tradeRow->getOfferID(), &findOfferRowByOfferId, &getOffersReader);
	if (!offer)
	{
	    throw std::runtime_error("Could not get offer table row.");
	}
	TradeInfo trade;
        trade.mInstrument = offer->getInstrument();
	trade.mTradeID = tradeRow->getTradeID();
        trade.mBuySell = tradeRow->getBuySell();
        trade.mOpenRate = tradeRow->getOpenRate();
        trade.mAmount = tradeRow->getAmount();
        trade.mOpenDate = toPtime(tradeRow->getOpenTime());
        trade.mGrossPL = tradeRow->getGrossPL();
	trades.push_back(trade);
        tradeRow->release();
    }
    return trades;
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
    O2G2Ptr<IO2GTableManager> tableManager = getLoadedTableManager();
    O2G2Ptr<IO2GTradesTable> tradesTable = static_cast<IO2GTradesTable*>(tableManager->getTable(Trades));
    IO2GTradeTableRow *tradeRow = NULL;
    IO2GTableIterator tableIterator;
    while (tradesTable->getNextRow(tableIterator, tradeRow)) {
	if (tradeID == tradeRow->getTradeID()) {
	    break;
	}
    }
    if (!tradeRow) {
	BOOST_LOG_TRIVIAL(error) << "Could not find trade with ID = " << tradeID;
        return false;
    }
    O2G2Ptr<IO2GValueMap> valuemap = mpRequestFactory->createValueMap();
    valuemap->setString(Command, O2G2::Commands::CreateOrder);
    valuemap->setString(OrderType, O2G2::Orders::TrueMarketClose);
    valuemap->setString(AccountID, mAccountID.c_str());
    valuemap->setString(OfferID, tradeRow->getOfferID());
    valuemap->setString(TradeID, tradeID.c_str());
    valuemap->setString(BuySell, (strcmp(tradeRow->getBuySell(), O2G2::Buy) == 0) ? O2G2::Sell : O2G2::Buy);
    tradeRow->release();
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
    O2G2Ptr<IO2GOfferRow> offer = getTableRow<IO2GOfferRow, IO2GOffersTableResponseReader>(Offers,
											   instrument,
											   &findOfferRowBySymbol,
											   &getOffersReader);
    if (!offer)
    {
	throw std::runtime_error("Could not get offer table row.");
    }
    return offer->getBid();
}

double ForexConnectClient::getAsk(const std::string& instrument) {
    O2G2Ptr<IO2GOfferRow> offer = getTableRow<IO2GOfferRow, IO2GOffersTableResponseReader>(Offers,
											   instrument,
											   &findOfferRowBySymbol,
											   &getOffersReader);
    if (!offer)
    {
	throw std::runtime_error("Could not get offer table row.");
    }
    return offer->getAsk();
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

template <class RowType, class ReaderType>
RowType* ForexConnectClient::getTableRow(O2GTable table, std::string key, bool (*finderFunc)(RowType *, std::string), ReaderType* (*readerCreateFunc)(IO2GResponseReaderFactory* , IO2GResponse *)) {

    O2G2Ptr<IO2GResponse> response = NULL;

    if(!mpLoginRules->isTableLoadedByDefault(table) ) {
        O2G2Ptr<IO2GRequest> request = mpRequestFactory->createRefreshTableRequestByAccount(Trades, mAccountID.c_str());
        if (!request) {
            return NULL;
        }
	mpResponseListener->setRequestID(request->getRequestID());
	mpSession->sendRequest(request);
	if (!mpResponseListener->waitEvents())
	{
	    return NULL;
	}
    } else {
        response = mpLoginRules->getTableRefreshResponse(table);
        if (!response) {
            return NULL;
        }
    }

    O2G2Ptr<ReaderType> reader = readerCreateFunc(mpResponseReaderFactory, response);

    RowType *row = NULL;

    for ( int i = 0; i < reader->size(); ++i ) {
        row = reader->getRow(i);
        if ( finderFunc(row, key) ) {
                break;
        }
        row->release();
        row = NULL;
    }
    return row;
}

IO2GTableManager* ForexConnectClient::getLoadedTableManager()
{
    O2G2Ptr<IO2GTableManager> tableManager = mpSession->getTableManager();
    O2GTableManagerStatus managerStatus = tableManager->getStatus();
    while (managerStatus == TablesLoading)
    {
	Sleep(50);
	managerStatus = tableManager->getStatus();
    }
    
    if (managerStatus == TablesLoadFailed)
    {
	throw std::runtime_error("Cannot refresh all tables of table manager");
    }
    return tableManager;
}
