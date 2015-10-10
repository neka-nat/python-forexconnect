#ifndef FOREXCONNECTCLIENT_H
#define FOREXCONNECTCLIENT_H

#include "ForexConnect.h"
#include "sample_tools.h"
#include <string>
#include <iostream>
#define _TIMEOUT 30000

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
	void printAccounts() const;
	bool sendEntryOrder();
	bool login();
	void logout();
    private:
	void init();
	LoginParams mLoginParams;
	IO2GSession* mpSession;
	SessionStatusListener* mpListener;
	bool mIsConnected;
    };
}

#endif
