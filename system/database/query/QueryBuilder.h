#ifndef _QUERY_BUILDER_H_
#define _QUERY_BUILDER_H_

#include <vector>
#include <unordered_map>

#include <system/database/query/WhereBinding.h>
#include <system/bundle/BindingType.h>
#include <system/database/query/JoinStatement.h>

#include <system/database/connections/QueryResult.h>

using namespace std;

namespace tuber{

class Binding;
class Grammar;
class Connection;

class QueryBuilder{
public:
	QueryBuilder(Connection* connection,Grammar* grammar);
	Binding* getBindings();

	string toSql();

	QueryResult runSelect();

	QueryRow first();
	QueryResult get();

	QueryBuilder* take(int limit);

	QueryBuilder* table(string table);

	QueryBuilder* distinct();
	QueryBuilder* select(vector<string> columns);
	QueryBuilder* selectDistinct(vector<string> columns);

	QueryBuilder* from(string table);

	QueryBuilder* join(string table,string statement,string joinMethod = "inner");

	QueryBuilder* where(string column,string operatorValue = "=",string rValue = "",string boolean = "and");
	QueryBuilder* where(string column,string operatorValue = "=",int rValue = 0,string boolean = "and");
	QueryBuilder* where(string column,string operatorValue = "=",bool rValue = false,string boolean = "and");
	QueryBuilder* where(string column,string operatorValue = "=",double rValue = 0.0,string boolean = "and");
	QueryBuilder* whereNull(string column,string operatorValue = "=",string boolean = "and");

	QueryBuilder* groupBy(string column);
	QueryBuilder* groupBy(vector<string> columns);

	QueryBuilder* having(string sql,string operatorValue = "and");

	QueryBuilder* orderBy(string column,string method = "asc");

	QueryBuilder* limit(int numRows,int offset);

	bool insert(Bundle* attributes);
	bool update(Bundle* attributes);
	bool deleteData();

	QueryBuilder* compileBundle(Bundle* bundle);

	int getLimit();
	int getOffset();
	bool getDistinct();
	string getFrom();
	string getTable();
	vector<string> getColumns();
	vector<string> getGroupByStatements();
	vector<JoinStatement> getJoinStatements();
	vector<WhereBinding> getWhereStatements();
	unordered_map<string,string> getOrderStatements();
	unordered_map<string,string> getHavingStatements();
private:
	Connection* m_connection;
	Grammar* m_grammar;
	Binding* m_bindings;

	vector<string> m_columns;
	string m_table;
	bool m_distinct = false;
	vector<JoinStatement> m_joins;
	vector<WhereBinding> m_where;
	vector<string> m_groupBy;
	unordered_map<string,string> m_having;
	unordered_map<string,string> m_orderBy;
	int m_offset;
	int m_limit;

	QueryBuilder* setWhereBinding(string column,string operatorValue,string boolean,BindingType type);
};

}

#endif
