#include <gtk/gtk.h>

#include <system/database/connectors/ConnectionFactory.h>

#include <system/database/connectors/Connector.h>
#include <system/database/connectors/SQLiteConnector.h>

#include <system/database/connections/Connection.h>
#include <system/database/connections/SQLiteConnection.h>

#include <system/foundation/InvalidArgumentException.h>

namespace tuber{

ConnectionFactory::ConnectionFactory(){

}

Connection* ConnectionFactory::make(map<string,string> driverConfig,string name){
	g_message("ConnectionFactory: Making connection for name [%s]",name.c_str());
	Connector* connector = this->createConnector(driverConfig["driver"]);
	return this->createConnection(driverConfig["driver"],connector,name,driverConfig["database"],driverConfig);
}

Connector* ConnectionFactory::createConnector(string driver){
	g_message("ConnectionFactory: Creating connector for driver [%s]",driver.c_str());
	if(driver == "sqlite"){
		return new SQLiteConnector();
	}

	throw InvalidArgumentException("Unsupported driver [" + driver + "]");
}

Connection* ConnectionFactory::createConnection(string driver,Connector* connector,string connectionName,string database,map<string,string> config){
	g_message("ConnectionFactory: Creating connection for driver [%s] and name [%s]",driver.c_str(),connectionName.c_str());
	if(driver == "sqlite"){
		return new SQLiteConnection(connector,connectionName,database,config);
	}

	throw InvalidArgumentException("Unsupported driver [" + driver + "]");
}


}
