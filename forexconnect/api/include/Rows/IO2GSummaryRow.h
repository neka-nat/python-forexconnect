
/*
    !!! Caution:
    Do not change anything in this source code because it was automatically generated
    from XML class description
*/
#pragma once

class IO2GSummaryRow : public IO2GRow
{
 protected:
    IO2GSummaryRow();
  
 public:

    
    //

};


class IO2GSummaryTableRow : public IO2GSummaryRow
{
 protected:
    IO2GSummaryTableRow();

 public:
    
    virtual const char* getOfferID() = 0;
    virtual int getDefaultSortOrder() = 0;
    virtual const char* getInstrument() = 0;
    virtual double getSellNetPL() = 0;
    virtual double getSellAmount() = 0;
    virtual double getSellAvgOpen() = 0;
    virtual double getBuyClose() = 0;
    virtual double getSellClose() = 0;
    virtual double getBuyAvgOpen() = 0;
    virtual double getBuyAmount() = 0;
    virtual double getBuyNetPL() = 0;
    virtual double getAmount() = 0;
    virtual double getGrossPL() = 0;
    virtual double getNetPL() = 0;
    //

};

