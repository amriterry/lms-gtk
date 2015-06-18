#include <gtk/gtk.h>

#include <system/database/connections/Connection.h>

#include <system/database/connections/statements/Statement.h>

namespace tuber{

Connection::Connection(Connector* connector,string connectionName,string database,map<string,string> config): m_connector(connector),m_connectionName(connectionName), m_database(database), m_config(config){

}

int Connection::insertQuery(string sql,Binding* bindings){
	return this->statement(sql,bindings);
}

int Connection::updateQuery(string sql,Binding* bindings){
	return this->statement(sql,bindings);
}

int Connection::deleteQuery(string sql,Binding* bindings){
	return this->statement(sql,bindings);
}

QueryResult Connection::selectQuery(string sql,Binding* bindings){
    g_message("Connection: Running Select Query: [%s]",sql.c_str());
	return this->resultStatement(sql,bindings);
}

void Connection::transaction(function<void()> callback){
	this->beginTransaction();
	try{
		callback();
		this->commit();
	} catch(exception &e){
		this->rollback();
	}
}

int Connection::affectedStatement(string query,Binding* bindings){
	return this->runAffected(query,bindings,[&](Statement* stmt,string query,Binding* bindings) -> int{
		return stmt->execute(query,bindings)->getAffectedId();
	});
}

int Connection::statement(string query,Binding* bindings){
	return this->run(query,bindings,[&](Statement* stmt,string query,Binding* bindings){
		return stmt->execute(query,bindings)->getAffectedNum();
	});
}

QueryResult Connection::resultStatement(string query,Binding* bindings){
	return this->runResult(query,bindings,[&](Statement* stmt,string query,Binding* bindings) -> QueryResult{
        g_message("Connection: ResultCallback");
		return stmt->executeAndFetch(query,bindings);
	});
}


}
