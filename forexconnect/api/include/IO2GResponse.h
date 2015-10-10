#pragma once

class Order2Go2 IO2GResponse : public IAddRef
{
 protected:
    IO2GResponse();
 public:
    virtual O2GResponseType getType() = 0;
    virtual const char * getRequestID() = 0;
};


class Order2Go2 IO2GResponseListener : public IAddRef
{
 protected:
    IO2GResponseListener();
 public:
    virtual void onRequestCompleted(const char * requestId, IO2GResponse  *response = 0) = 0;
    virtual void onRequestFailed(const char *requestId , const char *error) = 0;
    virtual void onTablesUpdates(IO2GResponse *tablesUpdates) = 0;
};


class Order2Go2 IO2GResponseReaderFactory : public IAddRef
{
 protected:
    IO2GResponseReaderFactory();
 public:
    /** Create table updates response reader.*/
    virtual IO2GTablesUpdatesReader *createTablesUpdatesReader(IO2GResponse *response) = 0;

    /** Create market data snapshot response.*/
    virtual IO2GMarketDataSnapshotResponseReader *createMarketDataSnapshotReader(IO2GResponse *response) = 0;

    /** Create market data response.*/
    virtual IO2GMarketDataResponseReader *createMarketDataReader(IO2GResponse *response) = 0;

    /** Create offers table response reader.*/
    virtual IO2GOffersTableResponseReader *createOffersTableReader(IO2GResponse *response)  = 0;

    /** Create Account table response reader.*/
    virtual IO2GAccountsTableResponseReader *createAccountsTableReader(IO2GResponse *response)  = 0;

    /** Create Order table response reader.*/
    virtual IO2GOrdersTableResponseReader *createOrdersTableReader(IO2GResponse *response)  = 0;

    /** Create trades table response reader.*/
    virtual IO2GTradesTableResponseReader *createTradesTableReader(IO2GResponse *response)  = 0;

    /** Create closed trades table response reader.*/
    virtual IO2GClosedTradesTableResponseReader *createClosedTradesTableReader(IO2GResponse *response)  = 0;

    /** Create messages table response reader.*/
    virtual IO2GMessagesTableResponseReader *createMessagesTableReader(IO2GResponse *response)  = 0;

    /** Create order response reader.*/
    virtual IO2GOrderResponseReader *createOrderResponseReader(IO2GResponse *response)  = 0;

    /** Create last order update response reader. */
    virtual IO2GLastOrderUpdateResponseReader *createLastOrderUpdateResponseReader(IO2GResponse *response) = 0;

    /** Create system properties reader.*/
    virtual IO2GSystemPropertiesReader *createSystemPropertiesReader(IO2GResponse *response) = 0;

    /** Process margin requirements response.*/
    virtual bool processMarginRequirementsResponse(IO2GResponse *response) = 0;
};

