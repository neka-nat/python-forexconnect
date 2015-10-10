
/*
    !!! Caution:
    Do not change anything in this source code because it was automatically generated
    from XML class description
*/
#pragma once

class IO2GMessageRow : public IO2GRow
{
 protected:
    IO2GMessageRow();
  
 public:

    
    virtual const char* getMsgID() = 0;
    virtual DATE getTime() = 0;
    virtual const char* getFrom() = 0;
    virtual const char* getType() = 0;
    virtual const char* getFeature() = 0;
    virtual const char* getText() = 0;
    virtual const char* getSubject() = 0;
    virtual bool getHTMLFragmentFlag() = 0;
    //

};


class IO2GMessageTableRow : public IO2GMessageRow
{
 protected:
    IO2GMessageTableRow();

 public:
    
    //

};

