
/*
    !!! Caution:
    Do not change anything in this source code because it was automatically generated
    from XML class description
*/
#pragma once

class IO2GOrderRow : public IO2GRow
{
 protected:
    IO2GOrderRow();
  
 public:

    
    virtual const char* getOrderID() = 0;
    virtual const char* getRequestID() = 0;
    virtual double getRate() = 0;
    virtual double getExecutionRate() = 0;
    virtual double getRateMin() = 0;
    virtual double getRateMax() = 0;
    virtual const char* getTradeID() = 0;
    virtual const char* getAccountID() = 0;
    virtual const char* getAccountName() = 0;
    virtual const char* getOfferID() = 0;
    virtual bool getNetQuantity() = 0;
    virtual const char* getBuySell() = 0;
    virtual const char* getStage() = 0;
    virtual const char* getType() = 0;
    virtual const char* getStatus() = 0;
    virtual DATE getStatusTime() = 0;
    virtual int getAmount() = 0;
    virtual double getLifetime() = 0;
    virtual double getAtMarket() = 0;
    virtual int getTrailStep() = 0;
    virtual double getTrailRate() = 0;
    virtual const char* getTimeInForce() = 0;
    virtual const char* getAccountKind() = 0;
    virtual const char* getRequestTXT() = 0;
    virtual const char* getContingentOrderID() = 0;
    virtual int getContingencyType() = 0;
    virtual const char* getPrimaryID() = 0;
    virtual int getOriginAmount() = 0;
    virtual int getFilledAmount() = 0;
    virtual bool getWorkingIndicator() = 0;
    virtual const char* getPegType() = 0;
    virtual double getPegOffset() = 0;
    virtual DATE getExpireDate() = 0;
    virtual const char* getValueDate() = 0;
    virtual const char* getParties() = 0;
    //

};


class IO2GOrderTableRow : public IO2GOrderRow
{
 protected:
    IO2GOrderTableRow();

 public:
    
    virtual double getStop() = 0;
    virtual double getLimit() = 0;
    virtual int getStopTrailStep() = 0;
    virtual double getStopTrailRate() = 0;
    //

};

