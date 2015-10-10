#include "ForexConnectClient.h"
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

/** Decrease reference counter. */
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
    if (!login())
    {
	throw std::runtime_error("Login fail.");
    }
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
    O2G2Ptr<IO2GResponseReaderFactory> readerFactory = mpSession->getResponseReaderFactory();
    if (!readerFactory)
    {
        std::cout << "Cannot create response reader factory" << std::endl;
        return;
    }
    O2G2Ptr<IO2GLoginRules> loginRules = mpSession->getLoginRules();
    O2G2Ptr<IO2GResponse> response = loginRules->getTableRefreshResponse(Accounts);
    O2G2Ptr<IO2GAccountsTableResponseReader> accountsResponseReader = readerFactory->createAccountsTableReader(response);
    std::cout.precision(2);
    for (int i = 0; i < accountsResponseReader->size(); ++i)
    {
        O2G2Ptr<IO2GAccountRow> accountRow = accountsResponseReader->getRow(i);
        std::cout << "AccountID: " << accountRow->getAccountID() << ", "
                << "Balance: " << std::fixed << accountRow->getBalance() << ", "
                << "Used margin: " << std::fixed << accountRow->getUsedMargin() << std::endl;
    }
}
