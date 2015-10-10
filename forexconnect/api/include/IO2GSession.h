#pragma once

class Order2Go2 IO2GSessionDescriptor : public IAddRef
{
 protected:
    IO2GSessionDescriptor();
 public:
    /** Gets the unique identifier of the descriptor. */
    virtual const char * getID() = 0;
    /** Gets the readable name of the descriptor. */
    virtual const char *getName() = 0;
    /** Gets the description of the descriptor. */
    virtual const char *getDescription() = 0;

    virtual bool requiresPin() = 0;
};

class Order2Go2 IO2GSessionDescriptorCollection : public IAddRef
{
 protected:
    IO2GSessionDescriptorCollection();
 public:
    /** Gets number of session descriptors. */
    virtual int size() = 0;
    /** Gets the session descriptor by index.*/
    virtual IO2GSessionDescriptor *get(int index) = 0;
};

class Order2Go2 IO2GSessionStatus : public IAddRef
{
 protected:
    IO2GSessionStatus();
 public:
    typedef enum
    {
        Disconnected = 0,
        Connecting = 1,
        TradingSessionRequested = 2,
        Connected = 3,
        Reconnecting = 4,
        Disconnecting = 5,
        SessionLost = 6,
        PriceSessionReconnecting = 7,
        ConnectedWithNeedToChangePassword = 8,
        ChartSessionReconnecting = 9,
    } O2GSessionStatus;

    virtual void onSessionStatusChanged(O2GSessionStatus status) = 0;
    virtual void onLoginFailed(const char *error) = 0;
};

class Order2Go2 IO2GChartSessionStatus : public IAddRef
{
 protected:
    IO2GChartSessionStatus();
 public:
    typedef enum
    {
        Disconnected = 0,
        Connecting = 1,        
        Connected = 3,
        Reconnecting = 4,
        Disconnecting = 5,
        SessionLost = 6,        
    } O2GChartSessionStatus;

    virtual void onChartSessionStatusChanged(O2GChartSessionStatus status) = 0;
    virtual void onChartSessionLoginFailed(const char *error) = 0;
};

class Order2Go2 IO2GTableManagerListener : public IAddRef
{
 protected:
    IO2GTableManagerListener();
 public:
    virtual void onStatusChanged(O2GTableManagerStatus status, IO2GTableManager *tableManager) = 0;
};



class Order2Go2 IO2GSession : public IAddRef
{
 protected:
    IO2GSession();
 public:
    virtual IO2GLoginRules *getLoginRules() = 0;
    /** Establishes connection with the trade server.*/
    virtual void login(const char *user, const char *pwd, const char *url, const char *connection) = 0;
    /** Closes connection with the trade server.*/
    virtual void logout() = 0;
    /* Subscribes the session status listener.*/
    virtual void subscribeSessionStatus(IO2GSessionStatus *listener) = 0;
    /* Unsubscribes the session status listener.*/
    virtual void unsubscribeSessionStatus(IO2GSessionStatus *listener) = 0;
    /** Gets the session descriptors collection.*/
    virtual IO2GSessionDescriptorCollection *getTradingSessionDescriptors() = 0;
    /** Sets the trading session identifier and pin.*/
    virtual void setTradingSession(const char *sessionId, const char *pin) = 0;
    /** Subscribes response listener.*/
    virtual void subscribeResponse(IO2GResponseListener *listener) = 0;
    /** Unsubscribes response listener.*/
    virtual void unsubscribeResponse(IO2GResponseListener *listener) = 0;
    /** Get the request factory.*/
    virtual IO2GRequestFactory * getRequestFactory() = 0 ;
    /** Gets the response factory reader.*/
    virtual IO2GResponseReaderFactory *getResponseReaderFactory() = 0;
    /** Send the request to the trade server.*/
    virtual void sendRequest(IO2GRequest *request) = 0;
    /** Gets time converter for converting request and markes snapshot date.*/
    virtual IO2GTimeConverter *getTimeConverter() = 0;
    /** Set session mode.*/
    virtual void setPriceUpdateMode(O2GPriceUpdateMode mode) = 0;
    /** Get session mode.*/
    virtual O2GPriceUpdateMode getPriceUpdateMode() = 0;
    /** Get server time.*/
    virtual DATE getServerTime() = 0;
    /** Gets report URL.

        @param  urlBuffer       [out] URL buffer.
        @param  bufferSize      Buffer size.
        @param  account         Account.
        @param  dateFrom        Start date of the report period.
        @param  dateTo          End date of the report period.
        @param  format          Report format.  HTML will be used in case of 0.
        @param  reportType      Report type.
        @param  langID          Language and locale of the report.
        @param  ansiCP          Code page.

        @return Number of characters written to the URL buffer if the function succeeds and urlBuffer is nonzero.
                The required size, in characters, for a buffer that can receive the URL string if the function succeeds and urlBuffer is zero.
                Negative value indicates failure. See O2GReportUrlError for possible values.
    */
    virtual int getReportURL(char* urlBuffer, int bufferSize, IO2GAccountRow* account,
        DATE dateFrom, DATE dateTo, const char* format, const char* reportType,
        const char* langID, long ansiCP) = 0;

    /** Get table manager.*/
    virtual IO2GTableManager *getTableManager() = 0;

    /** Get table manager by account.*/
    virtual IO2GTableManager *getTableManagerByAccount(const char *accountID) = 0;

    /** Set how to use table manager.*/
    virtual void useTableManager(O2GTableManagerMode mode, IO2GTableManagerListener *tablesListener) = 0;

    /** Gets current session status.*/
    virtual IO2GSessionStatus::O2GSessionStatus getSessionStatus() = 0;

    /** Gets the unique session identifier. */
    virtual const char *getSessionID() = 0;

    /** Sets the mode of the chart session. The method should be called before login. */
    virtual void setChartSessionMode(O2GChartSessionMode mode) = 0;
    /** Gets the mode of the chart session. */
    virtual O2GChartSessionMode getChartSessionMode() = 0;
    /* Subscribes the chart session status listener.*/
    virtual void subscribeChartSessionStatus(IO2GChartSessionStatus * listener) = 0;
    /* Unsubscribes the chart session status listener.*/
    virtual void unsubscribeChartSessionStatus(IO2GChartSessionStatus * listener) = 0;
    /** Gets current chart session status.*/
    virtual IO2GChartSessionStatus::O2GChartSessionStatus getChartSessionStatus() = 0;
};

