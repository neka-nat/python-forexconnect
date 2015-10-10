#pragma once
class Order2Go2 IO2GRequest :  public IAddRef
{
 protected:
    IO2GRequest();
 public:
    virtual const char *getRequestID() = 0;
    virtual int getChildrenCount() = 0;
    virtual IO2GRequest* getChildRequest(int index) = 0;
};

class Order2Go2 IO2GValueMap : public IAddRef
{
 protected:
    IO2GValueMap();
 public:

    virtual void setString(O2GRequestParamsEnum param, const char* value) = 0;
    virtual void setDouble(O2GRequestParamsEnum param , double value) = 0;
    virtual void setInt(O2GRequestParamsEnum param,  int value) = 0;
    virtual void setBoolean(O2GRequestParamsEnum param, bool value) = 0;

    virtual IO2GValueMap* clone() = 0;
    virtual void clear()  = 0;

    virtual int getChildrenCount() = 0;
    virtual IO2GValueMap* getChild(int index) = 0;
    virtual void appendChild(IO2GValueMap* valueMap) = 0;
};

/** Request factory.*/
class Order2Go2 IO2GRequestFactory : public IAddRef
{
 protected:
    IO2GRequestFactory();
 public:

    /** Gets time frames collection.*/
    virtual IO2GTimeframeCollection *getTimeFrameCollection() = 0;

    /** Create market a data snapshot request.*/
    virtual IO2GRequest *createMarketDataSnapshotRequestInstrument(const char *instrument, IO2GTimeframe *timeframe, int maxBars = 300) = 0;

    /** Fill parameters the market data snapshot*/
    virtual void fillMarketDataSnapshotRequestTime(IO2GRequest *request, DATE timeFrom = 0, DATE timeTo = 0, bool isIncludeWeekends = false) = 0;

    /** Gets refresh request.*/
    virtual IO2GRequest *createRefreshTableRequest(O2GTable table) = 0;

    /** Gets refresh request by the account.*/
    virtual IO2GRequest *createRefreshTableRequestByAccount(O2GTable table, const char* account) = 0;

    /** Create order request.*/
    virtual IO2GRequest *createOrderRequest(IO2GValueMap *valueMap) = 0;

    /** Create value map.*/
    virtual IO2GValueMap *createValueMap() = 0;

    /** Get last error.*/
    virtual const char* getLastError() = 0;
};

