#ifndef _STATEMENT_H_
#define _STATEMENT_H_

#include <system/database/connections/QueryResult.h>

namespace tuber{

class Binding;

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
};

}

#endif
