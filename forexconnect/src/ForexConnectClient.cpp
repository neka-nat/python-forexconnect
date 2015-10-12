#include "ForexConnectClient.h"
#include <string.h>
#include <stdexcept>
using namespace pyforexconnect;

static std::string FOREX_URL = "http://www40.fxcorporate.com/Hosts.jsp";

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
    std::cout << "Login error: " << error << std::endl;
    mError = true;
}

void SessionStatusListener::onSessionStatusChanged(IO2GSessionStatus::O2GSessionStatus status)
{
    switch (status)
    {
    case IO2GSessionStatus::Disconnected:
        std::cout << "status::disconnected" << std::endl;
        mConnected = false;
        mDisconnected = true;
        SetEvent(mSessionEvent);
        break;
    case IO2GSessionStatus::Connecting:
        std::cout << "status::connecting" << std::endl;
        break;
    case IO2GSessionStatus::TradingSessionRequested:
    {
        std::cout << "status::trading session requested" << std::endl;
        O2G2Ptr<IO2GSessionDescriptorCollection> descriptors = mSession->getTradingSessionDescriptors();
        bool found = false;
        if (descriptors)
        {
            if (mPrintSubsessions)
                std::cout << "descriptors available:" << std::endl;
            for (int i = 0; i < descriptors->size(); ++i)
            {
                O2G2Ptr<IO2GSessionDescriptor> descriptor = descriptors->get(i);
                if (mPrintSubsessions)
                    std::cout << "  id:='" << descriptor->getID()
                              << "' name='" << descriptor->getName()
                              << "' description='" << descriptor->getDescription()
                              << "' " << (descriptor->requiresPin() ? "requires pin" : "") << std::endl;
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
        std::cout << "status::connected" << std::endl;
        mConnected = true;
        mDisconnected = false;
        SetEvent(mSessionEvent);
        break;
    case IO2GSessionStatus::Reconnecting:
        std::cout << "status::reconnecting" << std::endl;
        break;
    case IO2GSessionStatus::Disconnecting:
        std::cout << "status::disconnecting" << std::endl;
        break;
    case IO2GSessionStatus::SessionLost:
        std::cout << "status::session lost" << std::endl;
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
    : mLoginParams(login, password, connection)
{
    init();
}

ForexConnectClient::ForexConnectClient(const LoginParams& loginParams)
    : mLoginParams(loginParams)
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

void ForexConnectClient::printAccounts() const
{
    std::cout << "AccountID: " << mpAccountRow->getAccountID() << ", "
	      << "Balance: " << std::fixed << mpAccountRow->getBalance() << ", "
	      << "Used margin: " << std::fixed << mpAccountRow->getUsedMargin() << std::endl;
}

std::vector<TradeInfo> ForexConnectClient::getTrades()
{
    std::vector<TradeInfo> trades;
    O2G2Ptr<IO2GTableManager> tableManager = getLoadedTableManager();

    O2G2Ptr<IO2GTradesTable> tradesTable = (IO2GTradesTable *)tableManager->getTable(Trades);
    IO2GTradeTableRow* tradeRow = NULL;
    IO2GTableIterator tableIterator;
    while (tradesTable->getNextRow(tableIterator, tradeRow)) {
        O2G2Ptr<IO2GOfferRow> offer = getTableRow<IO2GOfferRow, IO2GOffersTableResponseReader>(Offers, tradeRow->getOfferID(), &findOfferRowByOfferId, &getOffersReader);
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

template <class RowType, class ReaderType>
RowType* ForexConnectClient::getTableRow(O2GTable table, std::string key, bool (*finderFunc)(RowType *, std::string), ReaderType* (*readerCreateFunc)(IO2GResponseReaderFactory* , IO2GResponse *)) {

    O2G2Ptr<IO2GResponse> response;

    if(!mpLoginRules->isTableLoadedByDefault(table) ) {
        IO2GRequest *request = mpRequestFactory->createRefreshTableRequestByAccount(Trades, mAccountID.c_str());
        if (!response) {
            throw "No response to manual table refresh request";
        }
	mpResponseListener->setRequestID(request->getRequestID());
	mpSession->sendRequest(request);
	if (!mpResponseListener->waitEvents())
	{
	    throw "Response waiting timeout expired";
	}
    } else {
        response = mpLoginRules->getTableRefreshResponse(table);
        if (!response) {
            throw "No response to automatic table refresh request";
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
    if (row == NULL) {
        std::stringstream ss;
        ss << "Could not find row for key " << key;
        throw ss.str().c_str();
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
