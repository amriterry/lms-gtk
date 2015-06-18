#ifndef _SQLITE_CONNECTION_H_
#define _SQLITE_CONNECTION_H_

#include <vendor/sqlite/sqlite3.h>

#include <system/database/connections/Connection.h>

namespace tuber{

class SQLiteGrammar;

class SQLiteConnection: public Connection{
public:
	SQLiteConnection(Connector* connector,string connectionName,string database,map<string,string> config);

	virtual void beginTransaction();
	virtual void commit();
	virtual void rollback();

	virtual void execute(string query);
	virtual int runAffected(string query,Binding* bindings,RunAffectedCallback callback);
	virtual int run(string query,Binding* bindings,RunCallback callback);
	virtual QueryResult runResult(string query,Binding* bindings,RunResultCallback callback);

	virtual sqlite3* getRawConnection();
	virtual Grammar* getQueryGrammar();
	void setQueryGrammar(SQLiteGrammar* grammar);
protected:
	sqlite3* m_rawConnection;
	SQLiteGrammar* m_queryGrammar;

};

}

#endif
