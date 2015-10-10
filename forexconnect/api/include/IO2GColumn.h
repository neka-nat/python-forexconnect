#pragma once

class IO2GTableColumn : public IAddRef
{
 protected:
    IO2GTableColumn();
 public:
    typedef enum
    {
       Integer,
       Double,
       String,
       Date,
       Boolean
    }  O2GTableColumnType;
    /** Gets the unique identifier of the column.*/
    virtual const char * getID() = 0;
    /** Get the type of the column.*/
    virtual O2GTableColumnType getType() = 0;
};

class IO2GTableColumnCollection : public IAddRef
{
 protected:
    IO2GTableColumnCollection();
 public:
    /** Gets a number of the columns.*/
    virtual int size() = 0;
    /** Gets the column by the index.*/
    virtual IO2GTableColumn *get(int index) = 0;
    /** Find the column by the unique identifier.*/
    virtual IO2GTableColumn *find(const char *id) = 0;
};

