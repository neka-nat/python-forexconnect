#pragma once

class Order2Go2 IO2GMarketDataSnapshotResponseReader : public IAddRef
{
 protected:
    IO2GMarketDataSnapshotResponseReader();
 public:
    virtual bool isBar() = 0;
    /** Gets a number market rate.*/
    virtual int size() = 0;
    /** Get the date and time of the begin of the period.*/
    virtual DATE getDate(int index) = 0;
    /** Get the price of the bid tick.*/
    virtual double getBid(int index) = 0;
    /** Get the price of the ask tick.*/
    virtual double getAsk(int index) = 0;
    /** The open price of the bid bar or bid tick.*/
    virtual double getBidOpen(int index) = 0;
    /** Gets the highest prices of the bid bar.*/
    virtual double getBidHigh(int index) = 0;
    /** Gets the lowest price of the bid bar.*/
    virtual double getBidLow(int index) = 0;
    /** Gets the close price of the bid bar.*/
    virtual double getBidClose(int index) = 0;
    /** Gets the open price of the ask bar or ask tick.*/
    virtual double getAskOpen(int index) = 0;
    /** Gets the highest price of the ask bar.*/
    virtual double getAskHigh(int index) = 0;
    /** Gets the lowest price of the ask bar.*/
    virtual double getAskLow(int index) = 0;
    /** Gets the close price of the ask bar.*/
    virtual double getAskClose(int index) = 0;
    /** Gets the volume.*/
    virtual int getVolume(int index) = 0;
    /** Gets last bar volume. */
    virtual int getLastBarVolume() = 0;
    /** Gets last bar time.*/
    virtual DATE getLastBarTime() = 0;
};

