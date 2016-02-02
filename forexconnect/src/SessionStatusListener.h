#ifndef SESSIONSTATUSLISTENER_H
#define SESSIONSTATUSLISTENER_H


class SessionStatusListener : public IO2GSessionStatus
{
public:
    SessionStatusListener(IO2GSession *session,
			  bool printSubsessions = true,
			  const std::string& sessionID = "",
			  const std::string& pin = "");
protected:
    ~SessionStatusListener();
public:
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

#endif
