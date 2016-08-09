#include <gtk/gtk.h>

#include <system/database/DatabaseManager.h>

#include <system/database/connectors/ConnectionFactory.h>
#include <system/facades/Facades.h>

namespace tuber{

DatabaseManager::~DatabaseManager(){
	//...destroy all connection
}

Connection* DatabaseManager::makeConnection(string name){
	//g_message("DatabaseManager: Making connection of name [%s]",name.c_str());
	return this->m_factory->make(this->getConfig(name),name);
}

map<string,string> DatabaseManager::getConfig(string name){
	//g_message("DatabaseManager: Getting config for connection [%s]",name.c_str());
	return ConfigManagerFacade::get()->getConnectionConfiguration(name);
}

Connection* DatabaseManager::connection(string name){
	//g_message("DatabaseManager: getting connection");
	if(name == ""){
		name = this->getDefaultConnection();
	}

	//g_message("DatabaseManager: Getting connection with name [%s]",name.c_str());

	if(this->hasConnection(name)){
		return this->m_connections[name];
	}

	Connection* connection = this->makeConnection(name);
	//this->m_connections[name] = this->prepare(connection);

	return connection;
}

string DatabaseManager::getDefaultConnection(){
	return this->m_config->getDefaultDbConnection();
}

string DatabaseManager::getConnectionConfigValue(string key,string connectionName){
	if(connectionName == ""){
		connectionName = this->getDefaultConnection();
	}

	return this->m_config->getConnectionConfiguration(connectionName)[key];
}

bool DatabaseManager::hasConnection(string name){
	return (this->m_connections.find(name) != this->m_connections.end());
}

Connection* DatabaseManager::prepare(Connection* connection){
	//...prepare connection
	
	return connection;
}

}
