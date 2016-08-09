#ifndef _STATEMENT_H_
#define _STATEMENT_H_

#include <system/database/query/RowCallback.h>
#include <system/database/connections/QueryResult.h>

namespace tuber{

class Binding;
class Bundle;

class Statement{
public:
	virtual void* getRawStatement() = 0;
	virtual int getAffectedId() = 0;
	virtual int getAffectedNum() = 0;
	virtual Statement* prepare(string query) = 0;
	virtual Statement* bindData(Binding* bindings) = 0;
	virtual Statement* step(Binding* bindings) = 0;
	virtual Statement* execute(string query,Binding* bindings) = 0;
	virtual QueryResult executeAndFetch(string query,Binding* bindings) = 0;
	virtual void executeWithCallback(string query,Binding* bindings,RowCallback callback,Bundle* data = nullptr) = 0;
	virtual int getColumnCount() = 0;
	virtual int getColumnPos(string columnName) = 0;
	virtual map<string,int> getColumnNames() = 0;

	virtual int getInt(string columnName) = 0;
	virtual double getDouble(string columnName) = 0;
	virtual string getString(string columnName) = 0;
};

}

#endif
