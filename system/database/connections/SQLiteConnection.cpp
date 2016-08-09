#include <gtk/gtk.h>

#include <system/database/connections/SQLiteConnection.h>

#include <system/database/connectors/SQLiteConnector.h>
#include <system/database/connections/statements/SQLiteStatement.h>
#include <system/database/query/grammar/SQLiteGrammar.h>

namespace tuber{

SQLiteConnection::SQLiteConnection(Connector* connector,string connectionName,string database,map<string,string> config) : Connection(connector,connectionName,database,config){
	this->m_rawConnection = static_cast<SQLiteConnector*>(connector)->createRawConnection(config);
	this->m_queryGrammar = new SQLiteGrammar();
	//...Delete QueryGrammar
}

sqlite3* SQLiteConnection::getRawConnection(){
	return this->m_rawConnection;
}

Grammar* SQLiteConnection::getQueryGrammar(){
	return this->m_queryGrammar;
}

void SQLiteConnection::setQueryGrammar(SQLiteGrammar* grammar){
	this->m_queryGrammar = grammar;
}


void SQLiteConnection::beginTransaction(){
	this->statement("BEGIN TRANSACTION");
}

void SQLiteConnection::commit(){
	this->statement("COMMIT");
}

void SQLiteConnection::rollback(){
	this->statement("ROLLBACK");
}

void SQLiteConnection::execute(string query){
	this->statement(query);
}

int SQLiteConnection::runAffected(string query,Binding* bindings,RunAffectedCallback callback){
	SQLiteStatement* stmt = new SQLiteStatement(this->getRawConnection());
	int result = callback(stmt,query,bindings);
	delete stmt;
	return result;
}

int SQLiteConnection::run(string query,Binding* bindings,RunCallback callback){
	SQLiteStatement* stmt = new SQLiteStatement(this->getRawConnection());
	int val = callback(stmt,query,bindings);
	delete stmt;
	return val;
}

void SQLiteConnection::runCallback(string query,RowCallback closure,Binding* bindings,Bundle* data,RunWithCallback callback){
	SQLiteStatement* stmt = new SQLiteStatement(this->getRawConnection());
	callback(stmt,query,closure,bindings,data);
	delete stmt;
}

QueryResult SQLiteConnection::runResult(string query,Binding* bindings,RunResultCallback callback){
    g_message("SQLiteConnection: RunResult method");
	SQLiteStatement* stmt = new SQLiteStatement(this->getRawConnection());
	QueryResult result = callback(stmt,query,bindings);
	g_message("SQLiteConnection: Deleting statment");
	delete stmt;
	g_message("SQLiteConnection: Returning Result");
	return result;
}


}
