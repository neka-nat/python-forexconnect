#pragma once

class Order2Go2 IO2GMarketDataResponseReader : public IAddRef
{
 protected:
    IO2GMarketDataResponseReader();
 public:
    /** Gets QuoteID.*/
    virtual const char *getQuoteID() = 0;
    /** Gets Instrument.*/
    virtual const char *getInstrument() = 0;
    /** Gets Symbol ID (i.e. Offer ID).*/
    virtual int getSymbolID() = 0;
    /** Gets DateTime.*/
    virtual DATE getDateTime() = 0;

    /** Gets AskLow.*/
    virtual double getAskLow() = 0;
    /** Gets AskHigh.*/
    virtual double getAskHigh() = 0;
    /** Gets AskOpen.*/
    virtual double getAskOpen() = 0;
    /** Gets AskClose.*/
    virtual double getAskClose() = 0;

    /** Gets BidLow.*/
    virtual double getBidLow() = 0;
    /** Gets BidHigh.*/
    virtual double getBidHigh() = 0;
    /** Gets BidOpen.*/
    virtual double getBidOpen() = 0;
    /** Gets BidClose.*/
    virtual double getBidClose() = 0;

    /** Gets Low.*/
    virtual double getLow() = 0;
    /** Gets High.*/
    virtual double getHigh() = 0;

    /** Gets Timing Interval.*/
    virtual int getTimingInterval() = 0;
    /** Gets True if candle is completed, overwise False.*/
    virtual bool isCandleCompleted() = 0;
    /** Gets Market Data Request ID.*/
    virtual const char *getMarketDataRequestID() = 0;
    /** Gets Trading Session ID.*/
    virtual const char *getTradingSessionID() = 0;
    /** Gets Trading Session sub ID.*/
    virtual const char *getTradingSessionSubID() = 0;
    /** Gets Continuous Flag.*/
    virtual int getContinuosFlag() = 0;

    /** Gets Bid ID.*/
    virtual const char *getBidID() = 0;
    /** Gets Bid Quote Condition.*/
    virtual const char *getBidQuoteCondition() = 0;
    /** Gets Bid Quote Type.*/
    virtual int getBidQuoteType() = 0;
    /** Gets Bid Expire Date Time.*/
    virtual DATE getBidExpireDateTime() = 0;

    /** Gets Ask ID.*/
    virtual const char *getAskID() = 0;
    /** Gets Ask Quote Condition.*/
    virtual const char *getAskQuoteCondition() = 0;
    /** Gets Ask Quote Type.*/
    virtual int getAskQuoteType() = 0;
    /** Gets Ask Expire Date Time.*/
    virtual DATE getAskExpireDateTime() = 0;
};

