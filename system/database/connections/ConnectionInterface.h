#ifndef _CONNECTION_INTERFACE_H_
#define _CONNECTION_INTERFACE_H_

#include <string>

#include <system/database/connections/QueryResult.h>

using namespace std;

namespace tuber{

class Binding;

class ConnectionInterface{
public:
	virtual int insertQuery(string sql,Binding* bindings) = 0;
	virtual int updateQuery(string sql,Binding* bindings) = 0;
	virtual int deleteQuery(string sql,Binding* bindings) = 0;
	virtual QueryResult selectQuery(string sql,Binding* bindings) = 0;

	virtual void transaction(function<void()> callback) = 0;
};

}

#endif
