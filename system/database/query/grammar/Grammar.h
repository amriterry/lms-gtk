#ifndef _GRAMMAR_H_
#define _GRAMMAR_H_

#include <list>
#include <vector>
#include <string>

using namespace std;

namespace tuber{

class QueryBuilder;

class Grammar{
public:
	virtual string compileSelect(QueryBuilder* builder);
	virtual string compileColumns(QueryBuilder* builder);
	virtual string compileFrom(QueryBuilder* builder);
	virtual string compileJoin(QueryBuilder* builder);
	virtual string compileWhere(QueryBuilder* builder);
	virtual string compileGroupBy(QueryBuilder* builder);
	virtual string compileHaving(QueryBuilder* builder);
	virtual string compileOrderBy(QueryBuilder* builder);
	virtual string concatenateComponents(vector<string> strings,string glue);
	virtual string concatenateComponents(list<string> strings,string glue);

	virtual string compileInsert(QueryBuilder* builder);
	virtual string compileColumnValues(QueryBuilder* builder);

	virtual string compileUpdate(QueryBuilder* builder);
	virtual string compileNameValuePair(QueryBuilder* builder);

	virtual string compileDelete(QueryBuilder* builder);
};

}

#endif
