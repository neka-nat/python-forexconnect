
/*
    !!! Caution:
    Do not change anything in this source code because it was automatically generated
    from XML class description
*/
#pragma once

class IO2GClosedTradeRow : public IO2GRow
{
 protected:
    IO2GClosedTradeRow();
  
 public:

    
    virtual const char* getTradeID() = 0;
    virtual const char* getAccountID() = 0;
    virtual const char* getAccountName() = 0;
    virtual const char* getAccountKind() = 0;
    virtual const char* getOfferID() = 0;
    virtual int getAmount() = 0;
    virtual const char* getBuySell() = 0;
    virtual double getGrossPL() = 0;
    virtual double getCommission() = 0;
    virtual double getRolloverInterest() = 0;
    virtual double getOpenRate() = 0;
    virtual const char* getOpenQuoteID() = 0;
    virtual DATE getOpenTime() = 0;
    virtual const char* getOpenOrderID() = 0;
    virtual const char* getOpenOrderReqID() = 0;
    virtual const char* getOpenOrderRequestTXT() = 0;
    virtual const char* getOpenOrderParties() = 0;
    virtual double getCloseRate() = 0;
    virtual const char* getCloseQuoteID() = 0;
    virtual DATE getCloseTime() = 0;
    virtual const char* getCloseOrderID() = 0;
    virtual const char* getCloseOrderReqID() = 0;
    virtual const char* getCloseOrderRequestTXT() = 0;
    virtual const char* getCloseOrderParties() = 0;
    virtual const char* getTradeIDOrigin() = 0;
    virtual const char* getTradeIDRemain() = 0;
    virtual const char* getValueDate() = 0;
    //

};


class IO2GClosedTradeTableRow : public IO2GClosedTradeRow
{
 protected:
    IO2GClosedTradeTableRow();

 public:
    
    virtual double getPL() = 0;
    //

};

