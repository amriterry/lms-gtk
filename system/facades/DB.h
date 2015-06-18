#ifndef _DB_H_
#define _DB_H_

#include "Facade.h"
#include <system/database/DatabaseManager.h>
#include <system/database/connections/Connection.h>
#include <system/database/connections/QueryResult.h>

namespace tuber{

class DB: public Facade{
public:
	static DatabaseManager* get(){
		return m_app->resolve<DatabaseManager>(getFacadeAccessor());
	}

	static string getFacadeAccessor(){
		return "db";
	}

	static void raw(string query){
		get()->connection()->statement(query,nullptr);
	}

	static QueryResult rawSelect(string query){
		return get()->connection()->resultStatement(query,nullptr);
	}

	/*static QueryBuilder* table(string table){
		return get()->
	}*/
};

}

#endif
