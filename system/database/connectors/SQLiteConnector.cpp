#include <gtk/gtk.h>

#include <system/database/connectors/SQLiteConnector.h>

#include <system/facades/Facades.h>

#include <vendor/sqlite/sqlite3.h>

namespace tuber{

sqlite3* SQLiteConnector::createRawConnection(map<string,string> config){
	sqlite3* db;
	string path = PathFacade::get()->getPath("data") + config["database"];
	int rc = sqlite3_open(path.c_str(),&db);

	if(rc != SQLITE_OK){
		g_error("Could Not open database at: '%s' [%s]",path.c_str(),sqlite3_errmsg(db));
	}

	return db;
}

}
