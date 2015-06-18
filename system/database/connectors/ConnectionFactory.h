#ifndef _CONNECTION_FACTORY_H_
#define _CONNECTION_FACTORY_H_

#include <map>
#include <string>

#include <system/support/IBindable.h>

using namespace std;

namespace tuber{

class Connector;
class Connection;

class ConnectionFactory: public IBindable{
public:
	ConnectionFactory();
	Connection* make(map<string,string> driverConfig,string name);
	Connector* createConnector(string driver);
	Connection* createConnection(string driver,Connector* connector,string connectionName,string database,map<string,string> config);
};

}

#endif
