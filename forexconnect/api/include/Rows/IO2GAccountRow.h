
/*
    !!! Caution:
    Do not change anything in this source code because it was automatically generated
    from XML class description
*/
#pragma once

class IO2GAccountRow : public IO2GRow
{
 protected:
    IO2GAccountRow();
  
 public:

    
    virtual const char* getAccountID() = 0;
    virtual const char* getAccountName() = 0;
    virtual const char* getAccountKind() = 0;
    virtual double getBalance() = 0;
    virtual double getNonTradeEquity() = 0;
    virtual double getM2MEquity() = 0;
    virtual double getUsedMargin() = 0;
    virtual double getUsedMargin3() = 0;
    virtual const char* getMarginCallFlag() = 0;
    virtual DATE getLastMarginCallDate() = 0;
    virtual const char* getMaintenanceType() = 0;
    virtual int getAmountLimit() = 0;
    virtual int getBaseUnitSize() = 0;
    virtual bool getMaintenanceFlag() = 0;
    virtual const char* getManagerAccountID() = 0;
    virtual const char* getLeverageProfileID() = 0;
    //

};


class IO2GAccountTableRow : public IO2GAccountRow
{
 protected:
    IO2GAccountTableRow();

 public:
    
    virtual double getEquity() = 0;
    virtual double getDayPL() = 0;
    virtual double getUsableMargin() = 0;
    virtual double getGrossPL() = 0;
    //

};

