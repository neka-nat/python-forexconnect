#pragma once
class IO2GTableColumnCollection;

class IO2GRow : public IAddRef
{
 protected:
    IO2GRow();

 public:
    /** Gets the cell value.*/
    virtual const void *getCell(int column) = 0;

    /** Is cell changed?*/
    virtual bool isCellChanged(int column) = 0;

    /** Gets columns */
    virtual IO2GTableColumnCollection *columns() = 0;

    /** Gets table type */
    virtual O2GTable getTableType() = 0;
};

