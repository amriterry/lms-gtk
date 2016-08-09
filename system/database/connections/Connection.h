#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include <map>
#include <string>
#include <vector>
#include <functional>

#include <system/database/connections/ConnectionInterface.h>
#include <system/database/query/RowCallback.h>

using namespace std;

namespace tuber{

class Connector;
class Statement;
class Grammar;

typedef function<int(Statement*,string,Binding*)> RunAffectedCallback;
typedef function<int(Statement*,string,Binding*)> RunCallback;
typedef function<QueryResult(Statement*,string,Binding*)> RunResultCallback;
typedef function<void(Statement*,string,RowCallback,Binding*,Bundle*)> RunWithCallback;

class Connection: public ConnectionInterface{
public:
	virtual int insertQuery(string sql,Binding* bindings = nullptr);
	virtual int updateQuery(string sql,Binding* bindings = nullptr);
	virtual int deleteQuery(string sql,Binding* bindings = nullptr);
	virtual QueryResult selectQuery(string sql,Binding* bindings = nullptr);
	virtual void selectQuery(string sql,RowCallback closure,Binding* bindings = nullptr,Bundle* data = nullptr);

	virtual void transaction(function<void()> callback);
	virtual void beginTransaction() = 0;
	virtual void commit() = 0;
	virtual void rollback() = 0;

	virtual int affectedStatement(string query,Binding* bindings);
	virtual int statement(string query,Binding* bindings = nullptr);
	virtual void callbackStatement(string query,RowCallback closure,Binding* bindings,Bundle* data);
	virtual QueryResult resultStatement(string query,Binding* bindings);

	virtual void execute(string query) = 0;
	virtual int runAffected(string query,Binding* bindings,RunAffectedCallback callback) = 0;
	virtual int run(string query,Binding* bindings,RunCallback callback) = 0;
	virtual void runCallback(string query,RowCallback closure,Binding* bindings,Bundle* data,RunWithCallback callback) = 0;
	virtual QueryResult runResult(string query,Binding* bindings,RunResultCallback callback) = 0;

	virtual void* getRawConnection() = 0;
	virtual Grammar* getQueryGrammar() = 0;
protected:
	Connector* m_connector;
	string m_database;
	map<string,string> m_config;
	string m_connectionName;

	Connection(Connector* connector,string connectionName,string database,map<string,string> config);
};

}

#endif
