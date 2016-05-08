#include "TableHandler.h"
#include <stdexcept>

IO2GTableManager*
pyforexconnect::getLoadedTableManager(IO2GSession *session)
{
    O2G2Ptr<IO2GTableManager> tableManager = session->getTableManager();
    O2GTableManagerStatus managerStatus = tableManager->getStatus();
    while (managerStatus == TablesLoading)
    {
	Sleep(50);
	managerStatus = tableManager->getStatus();
    }

    if (managerStatus == TablesLoadFailed)
    {
	throw std::runtime_error("Cannot refresh all tables of table manager");
    }
    return tableManager.Detach();
}
