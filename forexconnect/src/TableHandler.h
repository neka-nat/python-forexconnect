#ifndef TABLEHANDLER_H
#define TABLEHANDLER_H
#include "stdafx.h"

namespace pyforexconnect
{
    IO2GTableManager* getLoadedTableManager(IO2GSession *session);

    template<O2GTable TableType, typename TableClass, typename TableRow>
    class TableHandler
    {
    public:
        TableHandler(IO2GSession *session)
	    : mTableManager(getLoadedTableManager(session)),
	    mTable(static_cast<TableClass*>(mTableManager->getTable(TableType))),
	    mpTableRow(NULL)
	{
	};

	~TableHandler()
	{
	    if (mpTableRow != NULL)
	    {
		mpTableRow->release();
	    }
	}

	TableRow* getNextRow()
	{
	    if (mpTableRow != NULL)
	    {
		mpTableRow->release();
	    }
	    if (mTable->getNextRow(mIterator, mpTableRow))
	    {
		return mpTableRow;
	    }
	    else
	    {
		return NULL;
	    }
	};

	TableRow* getRow()
	{
	    return mpTableRow;
	}

    private:
	O2G2Ptr<IO2GTableManager> mTableManager;
	O2G2Ptr<TableClass> mTable;
	IO2GTableIterator mIterator;
	TableRow* mpTableRow;
    };
}

#endif
