#ifndef _DATABASE_MANAGER_H_
#define _DATABASE_MANAGER_H_

#include <map>
#include <string>

#include <system/foundation/Service.h>
#include <system/database/IConnectionResolver.h>

using namespace std;

namespace tuber{

class Connection;
class ConfigManager;
class ConnectionFactory;

class DatabaseManager: public Service, public IConnectionResolver{
public:
	DatabaseManager(ConfigManager* config,ConnectionFactory* factory) : m_config(config),m_factory(factory){}
	virtual Connection* connection(string name = "");
	~DatabaseManager();
protected:
	Connection* makeConnection(string name = "");
	map<string,string> getConfig(string name);
	virtual string getDefaultConnection();
	string getConnectionConfigValue(string key,string connectionName = "");
	bool hasConnection(string name);
	Connection* prepare(Connection* connection);
private:
	ConnectionFactory* m_factory;
	ConfigManager* m_config;
	map<string,Connection*> m_connections;
};

}

#endif
