#include <gtk/gtk.h>

#include <system/database/query/QueryBuilder.h>

#include <system/database/connections/Connection.h>
#include <system/database/query/grammar/Grammar.h>
#include <system/database/Binding.h>
#include <system/facades/Facades.h>
#include <system/bundle/Bundle.h>

namespace tuber{

QueryBuilder::QueryBuilder(Connection* connection,Grammar* grammar): m_connection(connection), m_grammar(grammar){
	this->m_bindings = App::get()->resolve<Binding>("db.query.binding");
	this->m_offset = 0;
	this->m_limit = 0;
}

QueryResult QueryBuilder::runSelect(){
	return this->m_connection->selectQuery(this->toSql(),this->getBindings());
}

string QueryBuilder::toSql(){
	string sql = this->m_grammar->compileSelect(this);
	g_message("Query Running: [%s]",sql.c_str());
	return sql;
}

Binding* QueryBuilder::getBindings(){
	return this->m_bindings;
}

QueryRow QueryBuilder::first(){
	QueryResult result = this->take(1)->get();
	QueryRow row;
	if(result.size() != 0){
		row = result.front();
	}
	return row;
}

QueryResult QueryBuilder::get(){
	return this->runSelect();
}

QueryBuilder* QueryBuilder::take(int limit){
	this->m_limit = limit;
	return this;
}

QueryBuilder* QueryBuilder::table(string table){
	this->m_table = table;
	return this;
}

QueryBuilder* QueryBuilder::distinct(){
	this->m_distinct = true;
	return this;
}

QueryBuilder* QueryBuilder::select(vector<string> columns){
	this->m_columns = columns;
	return this;
}

QueryBuilder* QueryBuilder::selectDistinct(vector<string> columns){
	return this->distinct()->select(columns);
}

QueryBuilder* QueryBuilder::from(string table){
	return this->table(table);
}

QueryBuilder* QueryBuilder::join(string table,string statement,string joinMethod){
	JoinStatement stmt;

	stmt.table = table;
	stmt.statement = statement;
	stmt.joinMethod = joinMethod;

	this->m_joins.push_back(stmt);
	return this;
}

QueryBuilder* QueryBuilder::where(string column,string operatorValue,string rValue,string boolean){
	this->setWhereBinding(column,operatorValue,boolean,BindingType::STRING)->m_bindings->bindString(rValue);
	return this;
}

QueryBuilder* QueryBuilder::where(string column,string operatorValue,int rValue,string boolean){
	this->setWhereBinding(column,operatorValue,boolean,BindingType::INTEGER)->m_bindings->bindInteger(rValue);
	return this;
}

QueryBuilder* QueryBuilder::where(string column,string operatorValue,bool rValue,string boolean){
	this->setWhereBinding(column,operatorValue,boolean,BindingType::BOOLEAN)->m_bindings->bindBoolean(rValue);
	return this;
}

QueryBuilder* QueryBuilder::where(string column,string operatorValue,double rValue,string boolean){
	this->setWhereBinding(column,operatorValue,boolean,BindingType::DOUBLE)->m_bindings->bindDouble(rValue);
	return this;
}

QueryBuilder* QueryBuilder::whereNull(string column,string operatorValue,string boolean){
	this->setWhereBinding(column,operatorValue,boolean,BindingType::NULLTYPE)->m_bindings->bindNull();
	return this;
}

QueryBuilder* QueryBuilder::groupBy(string column){
	this->m_groupBy.push_back(column);
	return this;
}

QueryBuilder* QueryBuilder::groupBy(vector<string> columns){
	for(auto itr: columns){
		this->groupBy(itr);
	}
	return this;
}

QueryBuilder* QueryBuilder::having(string sql,string operatorValue){
	this->m_having.insert({sql,operatorValue});
	return this;
}

QueryBuilder* QueryBuilder::orderBy(string column,string method){
	this->m_orderBy.insert({column,method});
	return this;
}

QueryBuilder* QueryBuilder::limit(int numRows,int offset){
	this->m_offset = offset;
	this->m_limit = numRows;
	return this;
}

QueryBuilder* QueryBuilder::setWhereBinding(string column,string operatorValue,string boolean,BindingType type){
	int pos = this->m_bindings->getCurrentPosition();

	WhereBinding binding;
	binding.position = pos;
	binding.column = column;
	binding.operatorValue = operatorValue;
	binding.boolean = boolean;
	binding.type = type;

	this->m_where.push_back(binding);

	return this;
}

bool QueryBuilder::insert(Bundle* attributes){
	g_message("QueryBuilder: Insert query");
	this->compileBundle(attributes);
	int affected = this->m_connection->insertQuery(this->m_grammar->compileInsert(this),this->m_bindings);
	if(affected > 0){
		return true;
	} else {
		return false;
	}
}

bool QueryBuilder::update(Bundle* attributes){
	g_message("QueryBuilder: Update query");
	this->compileBundle(attributes);
	g_message("QueryBuilder: Bundle compiled now compiling query itself");
	int affected = this->m_connection->updateQuery(this->m_grammar->compileUpdate(this),this->m_bindings);
	if(affected > 0){
		return true;
	} else {
		return false;
	}
}

bool QueryBuilder::deleteData(){
	g_message("QueryBuilder: delete query");
	int affected = this->m_connection->deleteQuery(this->m_grammar->compileDelete(this),nullptr);
	if(affected > 0){
		return true;
	} else {
		return false;
	}
}

QueryBuilder* QueryBuilder::compileBundle(Bundle* bundle){
	int lastPos;
	for(auto it: bundle->getBindingPositionMap()){
		int pos = it.first;
		lastPos = pos;
		string key = it.second.first;
		g_message("QueryBuilder: position: [%i] ; key: [%s]",pos,key.c_str());
		switch(it.second.second){
			case BindingType::INTEGER:
				if(bundle->hasInt(key)){
					this->m_columns.push_back(key);
					this->m_bindings->bindInteger(bundle->getInt(key));
				}
				break;
			case BindingType::DOUBLE:
				if(bundle->hasDouble(key)){
					this->m_columns.push_back(key);
					this->m_bindings->bindDouble(bundle->getDouble(key));
				}
				break;
			case BindingType::BOOLEAN:
				if(bundle->hasDouble(key)){
					this->m_columns.push_back(key);
					this->m_bindings->bindDouble(bundle->getDouble(key));
				}
				break;
			case BindingType::STRING:
				if(bundle->hasString(key)){
					this->m_columns.push_back(key);
					this->m_bindings->bindString(bundle->getString(key));
				}
				break;
			case BindingType::NULLTYPE:
				if(bundle->hasGeneric(key)){
					this->m_columns.push_back(key);
					this->m_bindings->bindNull();
				}
				break;
			default:
				break;
		}
	}
	g_message("QueryBuilder: compiling Bundle");
	
	/*g_message("QueryBuilder: int map compiled");
	map<string,double> doubles = bundle->getDoubleMap();
	if(!doubles.empty()){
		for(auto it: doubles){
			this->m_columns.push_back(it.first);
			this->m_bindings->bindDouble(it.second);
		}
	}
	g_message("QueryBuilder: double map compiled");
	map<string,bool> booleans = bundle->getBooleanMap();
	if(!booleans.empty()){
		for(auto it: booleans){
			this->m_columns.push_back(it.first);
			this->m_bindings->bindBoolean(it.second);
		}
	}
	g_message("QueryBuilder: bool map compiled");
	map<string,string> stringMap = bundle->getStringMap();
	if(!stringMap.empty()){
		for(auto it: stringMap){
			this->m_columns.push_back(it.first);
			this->m_bindings->bindString(it.second);
		}
	}
	g_message("QueryBuilder: string map compiled");
	map<string,void*> nulls = bundle->getGenericMap();
	if(!nulls.empty()){
		for(auto it: nulls){
			this->m_columns.push_back(it.first);
			this->m_bindings->bindNull();
		}
	}*/
	g_message("QueryBuilder: Bundle compiled");
	return this;
}

int QueryBuilder::getLimit(){
	return this->m_limit;
}

int QueryBuilder::getOffset(){
	return this->m_offset;
}

bool QueryBuilder::getDistinct(){
	return this->m_distinct;
}

string QueryBuilder::getFrom(){
	return this->getTable();
}

string QueryBuilder::getTable(){
	return this->m_table;
}

vector<string> QueryBuilder::getColumns(){
	return this->m_columns;
}

vector<string> QueryBuilder::getGroupByStatements(){
	return this->m_groupBy;
}

vector<JoinStatement> QueryBuilder::getJoinStatements(){
	return this->m_joins;
}

vector<WhereBinding> QueryBuilder::getWhereStatements(){
	return this->m_where;
}

unordered_map<string,string> QueryBuilder::getOrderStatements(){
	return this->m_orderBy;
}

unordered_map<string,string> QueryBuilder::getHavingStatements(){
	return this->m_having;
}


}
