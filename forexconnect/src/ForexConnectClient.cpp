#include "ForexConnectClient.h"
#include "TableHandler.h"
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/bind.hpp>
#include <string.h>
#include <stdexcept>
#include <sstream>
#include <map>
#include <unistd.h>

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

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

LoginParams::LoginParams()
{
}

LoginParams::LoginParams(
    const std::string& login,
    const std::string& password,
    const std::string& connection,
    const std::string& url)
    : mLogin(login), mPassword(password), mConnection(connection),
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

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

ForexConnectClient::ForexConnectClient(
    const std::string& login,
    const std::string& password,
    const std::string& connection,
    const std::string& url)
      : mLoginParams(login, password, connection, url),
        mpSession(NULL), mpListener(NULL), mpResponseListener(NULL),
        mpLoginRules(NULL), mpAccountRow(NULL), mpResponseReaderFactory(NULL),
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

bool ForexConnectClient::isConnected() const
{
    return mIsConnected;
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

Prices::Prices()
    : mDate(boost::posix_time::second_clock::local_time()),
      mAskOpen(0.0), mAskHigh(0.0), mAskLow(0.0), mAskClose(0.0),
      mBidOpen(0.0), mBidHigh(0.0), mBidLow(0.0), mBidClose(0.0),
      mVolume(0)
{
}

Prices::Prices(boost::posix_time::ptime date, double value)
    : mDate(date), mAskOpen(value), mAskHigh(value), mAskLow(value), mAskClose(value),
      mBidOpen(value), mBidHigh(value), mBidLow(value), mBidClose(value), mVolume(value)
{
}

Prices::Prices(
boost::posix_time::ptime date,
         double askopen, double askhigh, double asklow, double askclose,
         double bidopen, double bidhigh, double bidlow, double bidclose,
         int volume)
    : mDate(date), mAskOpen(askopen), mAskHigh(askhigh), mAskLow(asklow),
      mAskClose(askclose), mBidOpen(bidopen), mBidHigh(bidhigh),
      mBidLow(bidlow), mBidClose(bidclose), mVolume(volume)
{
}

bool Prices::operator==(const Prices& other)
{
    return mDate == other.mDate &&
        mAskOpen == other.mAskOpen &&
        mAskHigh == other.mAskHigh &&
        mAskLow == other.mAskLow &&
        mAskClose == other.mAskClose &&
        mBidOpen == other.mBidOpen &&
        mBidHigh == other.mBidHigh &&
        mBidLow == other.mBidLow &&
        mBidClose == other.mBidClose &&
        mVolume == other.mVolume;
}

bool Prices::operator!=(const Prices& other)
{
    return !(*this == other);
}

std::ostream& pyforexconnect::operator<<(std::ostream& out, Prices const& pr)
{
    out << "<'date': " << pr.mDate
        << ", 'askopen': " << pr.mAskOpen
        << ", 'askhigh': " << pr.mAskHigh
        << ", 'asklow': " << pr.mAskLow
        << ", 'askclose': " << pr.mAskClose
        << ", 'bidopen': " << pr.mBidOpen
        << ", 'bidhigh': " << pr.mBidHigh
        << ", 'bidlow': " << pr.mBidLow
        << ", 'bidclose': " << pr.mBidClose
        << ", 'volume': " << pr.mVolume << ">";
    return out;
}

boost::python::list ForexConnectClient::getHistoricalPricesForPython(const std::string& instrument,
                     const double& from,
                     const double& to,
                     const std::string& timeFrame)
{
    return vector_to_python_list(getHistoricalPrices(instrument,
                 from,
                 to,
                 timeFrame));
}

std::vector<Prices> ForexConnectClient::getHistoricalPrices(const std::string& instrument,
                                                            const double& from,
                                                            const double& to,
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
    DATE dtFrom = from;
    DATE dtFirst = to;
    mpRequestFactory->fillMarketDataSnapshotRequestTime(request, dtFrom, dtFirst, false);
    mpResponseListener->setRequestID(request->getRequestID());
    mpSession->sendRequest(request);
    if (!mpResponseListener->waitEvents())
    {
        BOOST_LOG_TRIVIAL(error) << "Response waiting timeout expired";
        return prices;
    }
    O2G2Ptr<IO2GResponse> response = mpResponseListener->getResponse();
    if (response && response->getType() == MarketDataSnapshot)
    {
      O2G2Ptr<IO2GMarketDataSnapshotResponseReader> reader = mpResponseReaderFactory->createMarketDataSnapshotReader(response);
        if (reader->size() == 0)
        {
            BOOST_LOG_TRIVIAL(warning) << "0 rows received";
        }
        std::vector<Prices> prc = getPricesFromResponse(response);
        prices.insert(prices.end(), prc.begin(), prc.end());
    }
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
                  reader->getAskClose(ii),
                  reader->getBidOpen(ii),
                  reader->getBidHigh(ii),
                  reader->getBidLow(ii),
                  reader->getBidClose(ii),
                  reader->getVolume(ii)));
        }
        else
        {
            prices.push_back(Prices(toPtime(dt),
                             reader->getAsk(ii)));
        }
    }
    return prices;
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

std::map<std::string, std::string> ForexConnectClient::getOffers()
{
    std::map<std::string, std::string> offers;
    TableHandler<Offers, IO2GOffersTable, IO2GOfferTableRow> handler(mpSession);
    while (handler.getNextRow())
		{
        IO2GOfferTableRow *offerRow = handler.getRow();
        offers[offerRow->getInstrument()] = offerRow->getTradingStatus();
    }
    return offers;
}
std::map<std::string, double> ForexConnectClient::getTime()
{
    std::map<std::string, double> offers;
    TableHandler<Offers, IO2GOffersTable, IO2GOfferTableRow> handler(mpSession);
    while (handler.getNextRow())
		{
        IO2GOfferTableRow *offerRow = handler.getRow();
        offers[offerRow->getInstrument()] = offerRow->getTime();
    }
    return offers;
}

std::map<std::string, std::string> ForexConnectClient::getTradingStatus()
{
    std::map<std::string, std::string> offers;
    TableHandler<Offers, IO2GOffersTable, IO2GOfferTableRow> handler(mpSession);
    while (handler.getNextRow())
		{
        IO2GOfferTableRow *offerRow = handler.getRow();
        offers[offerRow->getInstrument()] = offerRow->getTradingStatus();
    }
    return offers;
}

boost::python::dict ForexConnectClient::getOffersForPython()
{
    return map_to_python_dict(getOffers());
}
boost::python::dict ForexConnectClient::getTimeForPython()
{
    return map_to_python_dict(getTime());
}
boost::python::dict ForexConnectClient::getTradingStatusForPython()
{
    return map_to_python_dict(getTradingStatus());
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
std::string ForexConnectClient::getOfferTradingStatus(const std::string& instrument) 
{
    TableHandler<Offers, IO2GOffersTable, IO2GOfferTableRow> handler(mpSession);
    while (handler.getNextRow())
    {
				IO2GOfferTableRow* offerRow = handler.getRow();
        if (offerRow->getInstrument() == instrument)
				{
	    			return offerRow->getTradingStatus();
				}
    }
    return "U";
}

double ForexConnectClient::getOfferTime(const std::string& instrument) 
{
    TableHandler<Offers, IO2GOffersTable, IO2GOfferTableRow> handler(mpSession);
    while (handler.getNextRow())
    {
				IO2GOfferTableRow* offerRow = handler.getRow();
        if (offerRow->getInstrument() == instrument)
				{
	    			return offerRow->getTime();
				}
    }
    return 0.0;
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

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void pyforexconnect::setLogLevel(int level)
{
    boost::log::core::get()->set_filter(boost::log::trivial::severity >= static_cast<boost::log::trivial::severity_level>(level));
}
