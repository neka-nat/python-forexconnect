#pragma once

#include "Rows/IO2GAccountRow.h"
#include "Rows/IO2GOfferRow.h"
#include "Rows/IO2GOrderRow.h"
#include "Rows/IO2GClosedTradeRow.h"
#include "Rows/IO2GTradeRow.h"
#include "Rows/IO2GMessageRow.h"
#include "Rows/IO2GSummaryRow.h"

/** Generic table response reader.*/
class Order2Go2 IO2GGenericTableResponseReader : public IAddRef
{
 protected:
    IO2GGenericTableResponseReader();
 public:
    /** Gets a number of the rows in the table.*/
    virtual int size() = 0;
    /** Gets the column of the table.*/
    virtual IO2GTableColumnCollection *columns() = 0;
    /** Gets the cell value.*/
    virtual const void *getCell(int row, int column) = 0;
    /** Check cell validation.*/
    virtual bool isCellValid(int row, int column) = 0;
    /** Gets table type.*/
    virtual O2GTable getType() = 0;
    /** Gets generic row*/
    virtual IO2GRow* getGenericRow(int rowIndex) = 0;
};

/** Offers table response reader */
class Order2Go2 IO2GOffersTableResponseReader : public IO2GGenericTableResponseReader
{
 protected:
    IO2GOffersTableResponseReader();
 public:
    virtual IO2GOfferRow *getRow(int index) = 0;
};

/** Accounts table response reader */
class Order2Go2 IO2GAccountsTableResponseReader : public IO2GGenericTableResponseReader
{
 protected:
    IO2GAccountsTableResponseReader();
 public:
    virtual IO2GAccountRow *getRow(int index) = 0;

};

/** Orders table response reader */
class Order2Go2 IO2GOrdersTableResponseReader : public IO2GGenericTableResponseReader
{
 protected:
    IO2GOrdersTableResponseReader();
 public:
    virtual IO2GOrderRow *getRow(int index) = 0;

};

/** Trades table response reader */
class Order2Go2 IO2GTradesTableResponseReader : public IO2GGenericTableResponseReader
{
 protected:
    IO2GTradesTableResponseReader();
 public:
    virtual IO2GTradeRow *getRow(int index) = 0;
};

/** Closed trades table response reader */
class Order2Go2 IO2GClosedTradesTableResponseReader : public IO2GGenericTableResponseReader
{
 protected:
    IO2GClosedTradesTableResponseReader();
 public:
    virtual IO2GClosedTradeRow *getRow(int index) = 0;
};

/** Messages table response reader */
class Order2Go2 IO2GMessagesTableResponseReader : public IO2GGenericTableResponseReader
{
 protected:
    IO2GMessagesTableResponseReader();
 public:
    virtual IO2GMessageRow *getRow(int index) = 0;
};

#ifndef DEFINED_IO2GTableIterator
#define DEFINED_IO2GTableIterator
/** Table rows iterator.*/
struct Order2Go2 IO2GTableIterator
{
    IO2GTableIterator()
    {
        reset();
    }

    ~IO2GTableIterator()
    {
        if (anchor)
            (*this.*release)();
    }

    void reset()
    {
        i = 0;
        j = 0;
        item = NULL;
        anchor = NULL;
        release = NULL;
    }

 private:
    unsigned i;
    unsigned j;
    void *item;
    void *anchor;
    void (IO2GTableIterator::*release)();
};
#endif

/** Table events listener.*/
class IO2GTableListener : public IAddRef
{
 public:
    virtual void onAdded(const char *rowID, IO2GRow *rowData) = 0;
    virtual void onChanged(const char *rowID, IO2GRow *rowData) = 0;
    virtual void onDeleted(const char *rowID, IO2GRow *rowData) = 0;

    virtual void onStatusChanged(O2GTableStatus status) = 0;
};

class IO2GEachRowListener : public IAddRef
{
 public:
    virtual void onEachRow(const char *rowID, IO2GRow *rowData) = 0;
};

/** Generic table interface.*/
class IO2GTable : public IO2GGenericTableResponseReader
{
 protected:
    IO2GTable();

 public:
    /** Subscribe to specific Update event.*/
    virtual void subscribeUpdate(O2GTableUpdateType updateType, IO2GTableListener *listener) = 0;
    /** Unsubscribe from specific Update event.*/
    virtual void unsubscribeUpdate(O2GTableUpdateType updateType, IO2GTableListener *listener) = 0;

    /** Subscribe to Status event.*/
    virtual void subscribeStatus(IO2GTableListener *listener) = 0;
    /** Unsubscribe from Status event.*/
    virtual void unsubscribeStatus(IO2GTableListener *listener) = 0;

    /** Get current table status.*/
    virtual O2GTableStatus getStatus() = 0;

    /** For each row.*/
    virtual void forEachRow(IO2GEachRowListener *listener) = 0;

    /** Is cell changed?*/
    virtual bool isCellChanged(int row, int column) = 0;

    /** Get next generic row by iteration.*/
    virtual bool getNextGenericRow(IO2GTableIterator &iterator, IO2GRow *&row) = 0;

    /** Get next generic row by specific column.*/
    virtual bool getNextGenericRowByColumnValue(const char *columnID, const void *columnValueAsVariant, IO2GTableIterator &iterator, IO2GRow *&row) = 0;

    /** Get next generic row by multi column.*/  
    virtual bool getNextGenericRowByMultiColumnValues(const int columnCount, const char *columnNames[], const void *columnValues[], IO2GTableIterator &iterator, IO2GRow *&row) = 0;

    /** Get next generic row by column values.*/  
    virtual bool getNextGenericRowByColumnValues(const char *columnName, const int valueCount, const void *columnValues[], IO2GTableIterator &iterator, IO2GRow *&row) = 0;
};

/** Table manager.*/
class IO2GTableManager : public IAddRef
{
 protected:
    IO2GTableManager();

 public:
    /** Get specific table.*/
    virtual IO2GTable *getTable(O2GTable tableType) = 0;
    virtual O2GTableManagerStatus getStatus() = 0;

    /** Lock all rows.*/
    virtual void lockUpdates() = 0;

    /** Unlock all rows.*/
    virtual void unlockUpdates() = 0;
};

/** Offers table.*/
class IO2GOffersTable : public IO2GTable
{
 protected:
    IO2GOffersTable();

 public:
    /** Get row specified by index.*/
    virtual IO2GOfferTableRow *getRow(int index) = 0;

    /** Get next row by iteration.*/
    virtual bool getNextRow(IO2GTableIterator &iterator, IO2GOfferTableRow *&row) = 0;

    /** Get next row by specific column.*/
    virtual bool getNextRowByColumnValue(const char *columnID, const void *columnValueAsVariant, IO2GTableIterator &iterator, IO2GOfferTableRow *&row) = 0;

    /** Find row specified by ID.*/
    virtual bool findRow(const char *id, IO2GOfferTableRow *&row) = 0;

    /** Get next row by multi column.*/
    virtual bool getNextRowByMultiColumnValues(const int columnCount, const char *columnNames[], const void *columnValues[], IO2GTableIterator &iterator, IO2GOfferTableRow *&row) = 0;

    /** Get next row by column values.*/
    virtual bool getNextRowByColumnValues(const char *columnName, const int valueCount, const void *columnValues[], IO2GTableIterator &iterator, IO2GOfferTableRow *&row) = 0;
};

/** Accounts table.*/
class IO2GAccountsTable : public IO2GTable
{
 protected:
    IO2GAccountsTable();

 public:
    /** Get row specified by index.*/
    virtual IO2GAccountTableRow *getRow(int index) = 0;

    /** Get next row by iteration.*/
    virtual bool getNextRow(IO2GTableIterator &iterator, IO2GAccountTableRow *&row) = 0;

    /** Get next row by specific column.*/
    virtual bool getNextRowByColumnValue(const char *columnID, const void *columnValueAsVariant, IO2GTableIterator &iterator, IO2GAccountTableRow *&row) = 0;

    /** Find row specified by ID.*/
    virtual bool findRow(const char *id, IO2GAccountTableRow *&row) = 0;

    /** Get next row by multi column.*/
    virtual bool getNextRowByMultiColumnValues(const int columnCount, const char *columnNames[], const void *columnValues[], IO2GTableIterator &iterator, IO2GAccountTableRow *&row) = 0;

    /** Get next row by column values.*/
    virtual bool getNextRowByColumnValues(const char *columnName, const int valueCount, const void *columnValues[], IO2GTableIterator &iterator, IO2GAccountTableRow *&row) = 0;
};

/** Orders table.*/
class IO2GOrdersTable : public IO2GTable
{
 protected:
    IO2GOrdersTable();

 public:
    /** Get row specified by index.*/
    virtual IO2GOrderTableRow *getRow(int index) = 0;

    /** Get next row by iteration.*/
    virtual bool getNextRow(IO2GTableIterator &iterator, IO2GOrderTableRow *&row) = 0;

    /** Get next row by specific column.*/
    virtual bool getNextRowByColumnValue(const char *columnID, const void *columnValueAsVariant, IO2GTableIterator &iterator, IO2GOrderTableRow *&row) = 0;

    /** Find row specified by ID.*/
    virtual bool findRow(const char *id, IO2GOrderTableRow *&row) = 0;

    /** Get next row by multi column.*/
    virtual bool getNextRowByMultiColumnValues(const int columnCount, const char *columnNames[], const void *columnValues[], IO2GTableIterator &iterator, IO2GOrderTableRow *&row) = 0;

    /** Get next row by column values.*/
    virtual bool getNextRowByColumnValues(const char *columnName, const int valueCount, const void *columnValues[], IO2GTableIterator &iterator, IO2GOrderTableRow *&row) = 0;
};

/** Trades table.*/
class IO2GTradesTable : public IO2GTable
{
 protected:
    IO2GTradesTable();

 public:
    /** Get row specified by index.*/
    virtual IO2GTradeTableRow *getRow(int index) = 0;

    /** Get next row by iteration.*/
    virtual bool getNextRow(IO2GTableIterator &iterator, IO2GTradeTableRow *&row) = 0;

    /** Get next row by specific column.*/
    virtual bool getNextRowByColumnValue(const char *columnID, const void *columnValueAsVariant, IO2GTableIterator &iterator, IO2GTradeTableRow *&row) = 0;

    /** Find row specified by ID.*/
    virtual bool findRow(const char *id, IO2GTradeTableRow *&row) = 0;

    /** Get next row by multi column.*/
    virtual bool getNextRowByMultiColumnValues(const int columnCount, const char *columnNames[], const void *columnValues[], IO2GTableIterator &iterator, IO2GTradeTableRow *&row) = 0;

    /** Get next row by column values.*/
    virtual bool getNextRowByColumnValues(const char *columnName, const int valueCount, const void *columnValues[], IO2GTableIterator &iterator, IO2GTradeTableRow *&row) = 0;
};

/** Closed trades table.*/
class IO2GClosedTradesTable : public IO2GTable
{
 protected:
    IO2GClosedTradesTable();

 public:
    /** Get row specified by index.*/
    virtual IO2GClosedTradeTableRow *getRow(int index) = 0;

    /** Get next row by iteration.*/
    virtual bool getNextRow(IO2GTableIterator &iterator, IO2GClosedTradeTableRow *&row) = 0;

    /** Get next row by specific column.*/
    virtual bool getNextRowByColumnValue(const char *columnID, const void *columnValueAsVariant, IO2GTableIterator &iterator, IO2GClosedTradeTableRow *&row) = 0;

    /** Find row specified by ID.*/
    virtual bool findRow(const char *id, IO2GClosedTradeTableRow *&row) = 0;

    /** Get next row by multi column.*/
    virtual bool getNextRowByMultiColumnValues(const int columnCount, const char *columnNames[], const void *columnValues[], IO2GTableIterator &iterator, IO2GClosedTradeTableRow *&row) = 0;

    /** Get next row by column values.*/
    virtual bool getNextRowByColumnValues(const char *columnName, const int valueCount, const void *columnValues[], IO2GTableIterator &iterator, IO2GClosedTradeTableRow *&row) = 0;
};

/** Messages table.*/
class IO2GMessagesTable : public IO2GTable
{
 protected:
    IO2GMessagesTable();

 public:
    /** Get row specified by index.*/
    virtual IO2GMessageTableRow *getRow(int index) = 0;

    /** Get next row by iteration.*/
    virtual bool getNextRow(IO2GTableIterator &iterator, IO2GMessageTableRow *&row) = 0;

    /** Get next row by specific column.*/
    virtual bool getNextRowByColumnValue(const char *columnID, const void *columnValueAsVariant, IO2GTableIterator &iterator, IO2GMessageTableRow *&row) = 0;

    /** Find row specified by ID.*/
    virtual bool findRow(const char *id, IO2GMessageTableRow *&row) = 0;

    /** Get next row by multi column.*/
    virtual bool getNextRowByMultiColumnValues(const int columnCount, const char *columnNames[], const void *columnValues[], IO2GTableIterator &iterator, IO2GMessageTableRow *&row) = 0;

    /** Get next row by column values.*/
    virtual bool getNextRowByColumnValues(const char *columnName, const int valueCount, const void *columnValues[], IO2GTableIterator &iterator, IO2GMessageTableRow *&row) = 0;
};

/** Summary table.*/
class IO2GSummaryTable : public IO2GTable
{
 protected:
    IO2GSummaryTable();

 public:
    /** Get row specified by index.*/
    virtual IO2GSummaryTableRow *getRow(int index) = 0;

    /** Get next row by iteration.*/
    virtual bool getNextRow(IO2GTableIterator &iterator, IO2GSummaryTableRow *&row) = 0;

    /** Get next row by specific column.*/
    virtual bool getNextRowByColumnValue(const char *columnID, const void *columnValueAsVariant, IO2GTableIterator &iterator, IO2GSummaryTableRow *&row) = 0;

    /** Find row specified by ID.*/
    virtual bool findRow(const char *id, IO2GSummaryTableRow *&row) = 0;

    /** Get next row by multi column.*/
    virtual bool getNextRowByMultiColumnValues(const int columnCount, const char *columnNames[], const void *columnValues[], IO2GTableIterator &iterator, IO2GSummaryTableRow *&row) = 0;

    /** Get next row by column values.*/
    virtual bool getNextRowByColumnValues(const char *columnName, const int valueCount, const void *columnValues[], IO2GTableIterator &iterator, IO2GSummaryTableRow *&row) = 0;
};



