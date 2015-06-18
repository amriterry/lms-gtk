#ifndef _SQLITE_STATEMENT_H_
#define _SQLITE_STATEMENT_H_

#include <vendor/sqlite/sqlite3.h>

#include <system/database/connections/statements/Statement.h>

namespace tuber{

class SQLiteStatement: public Statement{
public:
	SQLiteStatement(sqlite3* connection);
	virtual sqlite3_stmt* getRawStatement();
	virtual int getAffectedId();
	virtual int getAffectedNum();
	virtual Statement* prepare(string query);
	virtual Statement* bindData(Binding* bindings);
	virtual Statement* step(Binding* bindings);
	virtual Statement* execute(string query,Binding* bindings);
	virtual QueryResult executeAndFetch(string query,Binding* bindings);
	virtual ~SQLiteStatement();
protected:
	sqlite3* m_connection;
	sqlite3_stmt* m_stmt;
	QueryResult m_result;
};

}

#endif
