#include "stdafx.h"
#include <boost/log/trivial.hpp>
#include <string.h>
#include <stdexcept>

#include "SessionStatusListener.h"

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
    BOOST_LOG_TRIVIAL(trace) << __PRETTY_FUNCTION__;
    return InterlockedIncrement(&mRefCount);
}

long SessionStatusListener::release()
{
    BOOST_LOG_TRIVIAL(trace) << __PRETTY_FUNCTION__;
    long rc = InterlockedDecrement(&mRefCount);
    if (rc == 0)
        delete this;
    return rc;
}

void SessionStatusListener::reset()
{
    BOOST_LOG_TRIVIAL(trace) << __PRETTY_FUNCTION__;
    mConnected = false;
    mDisconnected = false;
    mError = false;
}

void SessionStatusListener::onLoginFailed(const char *error)
{
    BOOST_LOG_TRIVIAL(trace) << __PRETTY_FUNCTION__;
    BOOST_LOG_TRIVIAL(error) << "Login error: " << error;
    mError = true;
}

void SessionStatusListener::onSessionStatusChanged(IO2GSessionStatus::O2GSessionStatus status)
{
    BOOST_LOG_TRIVIAL(trace) << __PRETTY_FUNCTION__;
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
    BOOST_LOG_TRIVIAL(trace) << __PRETTY_FUNCTION__;
    return mError;
}

bool SessionStatusListener::isConnected() const
{
    BOOST_LOG_TRIVIAL(trace) << __PRETTY_FUNCTION__;
    return mConnected;
}

bool SessionStatusListener::isDisconnected() const
{
    BOOST_LOG_TRIVIAL(trace) << __PRETTY_FUNCTION__;
    return mDisconnected;
}

bool SessionStatusListener::waitEvents()
{
    BOOST_LOG_TRIVIAL(trace) << __PRETTY_FUNCTION__;
    return WaitForSingleObject(mSessionEvent, _TIMEOUT) == 0;
}
