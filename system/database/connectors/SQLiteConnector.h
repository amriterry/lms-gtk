#ifndef _SQLITE_CONNECTOR_H_
#define _SQLITE_CONNECTOR_H_

#include <system/database/connectors/Connector.h>

#include <vendor/sqlite/sqlite3.h>

namespace tuber{

class SQLiteConnector: public Connector{
public:
	sqlite3* createRawConnection(map<string,string> config);
};

}

#endif