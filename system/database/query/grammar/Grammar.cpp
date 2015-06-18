#include <gtk/gtk.h>

#include <unordered_map>

#include <system/database/query/grammar/Grammar.h>

#include <system/database/query/QueryBuilder.h>

using namespace std;

namespace tuber{

string Grammar::compileSelect(QueryBuilder* builder){
	vector<string> sql = {"select"};

	if(builder->getDistinct()){
		sql.push_back("distinct");
	}

	sql.push_back(this->compileColumns(builder));
	sql.push_back(this->compileFrom(builder));
	sql.push_back(this->compileJoin(builder));
	sql.push_back(this->compileWhere(builder));
	sql.push_back(this->compileGroupBy(builder));
	sql.push_back(this->compileHaving(builder));
	sql.push_back(this->compileOrderBy(builder));

	return this->concatenateComponents(sql," ");
}

string Grammar::compileColumns(QueryBuilder* builder){
	return this->concatenateComponents(builder->getColumns(),",");
}

string Grammar::compileFrom(QueryBuilder* builder){
	return string("from ") + builder->getFrom();
}

string Grammar::compileJoin(QueryBuilder* builder){
	vector<JoinStatement> joins = builder->getJoinStatements();
	if(joins.size()){
		vector<string> joinStmt;
		for(auto stmt: joins){
			joinStmt.push_back(stmt.joinMethod + string(" join ") + stmt.table + string(" on ") + stmt.statement);
		}
		return this->concatenateComponents(joinStmt,",");
	}
	return "";
}

string Grammar::compileWhere(QueryBuilder* builder){
	vector<WhereBinding> whereClauses = builder->getWhereStatements();
	if(whereClauses.size()){
		vector<string> whereStmt;
		whereStmt.push_back("where");
		for(vector<WhereBinding>::iterator clause = whereClauses.begin();clause != whereClauses.end(); ++clause){
			string stmt;
			if(clause != whereClauses.begin()){
				stmt += clause->boolean;
			}
			stmt += clause->column + string(" ") + clause->operatorValue + string(" ?");
			whereStmt.push_back(stmt);
		}
		return this->concatenateComponents(whereStmt," ");
	}
	return "";
}

string Grammar::compileGroupBy(QueryBuilder* builder){
	vector<string> groupByClause = builder->getGroupByStatements();
	if(groupByClause.size()){
		return string("group by ") + this->concatenateComponents(groupByClause,",");
	}
	return "";
}

string Grammar::compileHaving(QueryBuilder* builder){
	unordered_map<string,string> havings = builder->getHavingStatements();
	if(havings.size()){
		list<string> havingClause;
		for(auto clause: havings){
			string clauseString;
			if(clause != *(havings.begin())){
				clauseString += clause.second;
			}
			clauseString += clause.first;
			havingClause.push_front(clauseString);
		}
		return string("having ") + this->concatenateComponents(havingClause," ");
	}
	return "";
}

string Grammar::compileOrderBy(QueryBuilder* builder){
	unordered_map<string,string> orders = builder->getOrderStatements();
	if(orders.size()){
		list<string> orderClause;
		for(auto clause: orders){
			orderClause.push_front(clause.first + string(" ") + clause.second);
		}
		return string("order by ") + this->concatenateComponents(orderClause,",");
	}
	return "";
}

string Grammar::compileInsert(QueryBuilder* builder){
	vector<string> sql = {"insert into"};
	sql.push_back(builder->getTable());
	sql.push_back(string("(") + this->compileColumns(builder) + string(")"));
	sql.push_back(string("values"));
	sql.push_back(string("(") + this->compileColumnValues(builder) + string(")"));

	string compiled = this->concatenateComponents(sql," ");
	g_message("Grammar: compiled query: \n\n[%s]\n\n",compiled.c_str());
	return compiled;
}

string Grammar::compileUpdate(QueryBuilder* builder){
	vector<string> sql = {"update"};
	sql.push_back(builder->getTable());
	sql.push_back("set");
	sql.push_back(this->compileNameValuePair(builder));
	sql.push_back(this->compileWhere(builder));

	string compiled = this->concatenateComponents(sql," ");
	g_message("Grammar: compiled query: \n\n[%s]\n\n",compiled.c_str());
	return compiled;
}

string Grammar::compileDelete(QueryBuilder* builder){
	vector<string> sql = {"delete"};
	sql.push_back(this->compileFrom(builder));
	sql.push_back(this->compileWhere(builder));

	return this->concatenateComponents(sql," ");
}

string Grammar::compileColumnValues(QueryBuilder* builder){
	int colSize = builder->getColumns().size();
	string value;
	for(int i = 0;i < colSize;i++){
		value += "?";
		if(i != colSize-1){
			value += ",";
		}
	}
	return value;
}

string Grammar::compileNameValuePair(QueryBuilder* builder){
	vector<string> columns = builder->getColumns();
	vector<string> nameValue;
	for(auto it: columns){
		nameValue.push_back(it + " = ?");
	}
	return this->concatenateComponents(nameValue,",");
}

string Grammar::concatenateComponents(vector<string> strings,string glue){
	string concatenated;
	for(auto element: strings){
		if(element != strings.back() && element != ""){
			element += glue;
		}
		concatenated += element;
	}
	return concatenated;
}

string Grammar::concatenateComponents(list<string> strings,string glue){
	string concatenated;
	for(auto element: strings){
		if(element != strings.back() && element != ""){
			element += glue;
		}
		concatenated += element;
	}
	return concatenated;
}

}
