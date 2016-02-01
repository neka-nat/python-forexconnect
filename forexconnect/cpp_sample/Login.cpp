#include "stdafx.h"
#include "SessionStatusListener.h"

bool login(IO2GSession *session,
	   SessionStatusListener *statusListener,
	   const std::string& login,
	   const std::string& pass,
	   const std::string& url,
	   const std::string& connection)
{
    statusListener->reset();
    session->login(login.c_str(), pass.c_str(),
		   url.c_str(), connection.c_str());
    return statusListener->waitEvents() && statusListener->isConnected();
}

void logout(IO2GSession *session, SessionStatusListener *statusListener)
{
    statusListener->reset();
    session->logout();
    statusListener->waitEvents();
}

int main(int argc, char *argv[])
{
    if (argc < 5)
    {
	std::cout << "Usage: login <username> <password> <url> <Real/Demo>" << std::endl;
	return -1;
    }

    IO2GSession *session = CO2GTransport::createSession();

    SessionStatusListener *sessionListener = new SessionStatusListener(session, false);
    session->subscribeSessionStatus(sessionListener);

    bool bConnected = login(session, sessionListener,
			    argv[1],
			    argv[2],
			    argv[3],
			    argv[4]);
    bool bWasError = false;

    if (bConnected)
    {
        std::cout << "Done!" << std::endl;
        logout(session, sessionListener);
    }
    else
    {
        bWasError = true;
    }

    session->unsubscribeSessionStatus(sessionListener);
    sessionListener->release();
    session->release();

    if (bWasError)
        return -1;
    return 0;
}
