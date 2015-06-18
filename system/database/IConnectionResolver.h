#ifndef _I_CONNECTION_RESOLVER_H_
#define _I_CONNECTION_RESOLVER_H_

#include <string>

using namespace std;

namespace tuber{

class Connection;

class IConnectionResolver{
public:
	virtual Connection* connection(string name = "") = 0;
	virtual string getDefaultConnection() = 0;
};

}

#endif