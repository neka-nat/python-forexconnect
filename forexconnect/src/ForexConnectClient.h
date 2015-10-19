#ifndef FOREXCONNECTCLIENT_H
#define FOREXCONNECTCLIENT_H

#include "stdafx.h"
#include "ResponseListener.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/python/list.hpp>
#include <boost/python/dict.hpp>
#include <sstream>
#include <iomanip>

namespace pyforexconnect
{
    struct LoginParams
    {
	std::string mLogin;
	std::string mPassword;
	std::string mConnection;
	LoginParams();
	LoginParams(const std::string& login,
		    const std::string& password,
		    const std::string& connection);
    };
    std::ostream& operator<<(std::ostream& out, LoginParams const& lp);

    struct TradeInfo
    {
        std::string mInstrument;
	std::string mTradeID;
	std::string mBuySell;
        double mOpenRate;
        int mAmount;
        boost::posix_time::ptime mOpenDate;
        double mGrossPL;
	bool operator==(const TradeInfo& other);
	bool operator!=(const TradeInfo& other);
	TradeInfo();
    };
    std::ostream& operator<<(std::ostream& out, TradeInfo const& ti);

    struct Prices
    {
        boost::posix_time::ptime mDate;
	double mOpen;
	double mHigh;
	double mLow;
	double mClose;
	bool operator==(const Prices& other);
	bool operator!=(const Prices& other);
	Prices();
	Prices(boost::posix_time::ptime date,
	       double value);
	Prices(boost::posix_time::ptime date,
	       double open,
	       double high,
	       double low,
	       double close);
    };
    std::ostream& operator<<(std::ostream& out, Prices const& pr);

    class SessionStatusListener : public IO2GSessionStatus
    {
    public:
	SessionStatusListener(IO2GSession *session,
			      bool printSubsessions = true,
			      const std::string& sessionID = "",
			      const std::string& pin = "");
	~SessionStatusListener();
	virtual long addRef();
	virtual long release();
	virtual void onLoginFailed(const char *error);
	virtual void onSessionStatusChanged(IO2GSessionStatus::O2GSessionStatus status);
	bool hasError() const;
	bool isConnected() const;
	bool isDisconnected() const;
	void reset();
	bool waitEvents();
    private:
	long mRefCount;
	std::string mSessionID;
        std::string mPin;
        bool mError;
        bool mConnected;
        bool mDisconnected;
        bool mPrintSubsessions;
        IO2GSession* mSession;
        HANDLE mSessionEvent;
    };

    class ForexConnectClient
    {
    public:
	ForexConnectClient(const std::string& login,
			   const std::string& password,
			   const std::string& connection);
	ForexConnectClient(const LoginParams& loginParams);
	~ForexConnectClient();
	std::vector<TradeInfo> getTrades();
	boost::python::list getTradesForPython();
	bool openPosition(const std::string& instrument,
			  const std::string& buysell,
			  int amount);
	bool closePosition(const std::string& tradeID);
	double getBid(const std::string& instrument);
	double getAsk(const std::string& instrument);
	std::vector<Prices> getHistoricalPrices(const std::string& instrument,
						const boost::posix_time::ptime& from,
						const boost::posix_time::ptime& to,
						const std::string& timeFrame = std::string("m1"));
	boost::python::list getHistoricalPricesForPython(const std::string& instrument,
							 const boost::posix_time::ptime& from,
							 const boost::posix_time::ptime& to,
							 const std::string& timeFrame = std::string("m1"));
	std::string getAccountID() const;
	double getUsedMargin() const;
	double getBalance() const;
	std::map<std::string, std::string> getOffers();
	boost::python::dict getOffersForPython();
	bool isConnected() const;
	bool login();
	void logout();
    private:
	void init();
	IO2GTableManager* getLoadedTableManager();
	std::vector<Prices> getPricesFromResponse(IO2GResponse* response);
	LoginParams mLoginParams;
	IO2GSession* mpSession;
	SessionStatusListener* mpListener;
        ResponseListener* mpResponseListener;
        IO2GLoginRules* mpLoginRules;
        IO2GAccountRow* mpAccountRow;
        IO2GResponseReaderFactory* mpResponseReaderFactory;
        IO2GRequestFactory* mpRequestFactory;
        std::string mAccountID;
	bool mIsConnected;

        static boost::posix_time::ptime toPtime(double d)
	{
            double d_int, d_frac;
            d_frac = modf(d, &d_int);
            time_t t = time_t(d_int - 25569.0) * 86400 + time_t(floor((d_frac * 86400) + 0.5));
            return boost::posix_time::from_time_t(t);
        }

	static double toOleTime(const boost::posix_time::ptime& t)
	{
	    double d;
	    struct tm ttm = boost::posix_time::to_tm(t);
	    CO2GDateUtils::CTimeToOleTime(&ttm, &d);
	    return d;
	}

        static bool findOfferRowBySymbol(IO2GOfferRow *row, std::string symbol)
	{
            return (symbol == row->getInstrument() && row->getSubscriptionStatus()[0] == 'T');
        }

        static bool findOfferRowByOfferId(IO2GOfferRow *row, std::string offerId)
	{
            return (offerId == row->getOfferID());
        }

        static IO2GOffersTableResponseReader* getOffersReader(IO2GResponseReaderFactory* readerFactory,
							      IO2GResponse *response)
	{
            return readerFactory->createOffersTableReader(response);
        }
	
        static IO2GTradesTableResponseReader* getTradesReader(IO2GResponseReaderFactory* readerFactory,
							      IO2GResponse *response)
	{
            return readerFactory->createTradesTableReader(response);
        }
    };
}

#endif
