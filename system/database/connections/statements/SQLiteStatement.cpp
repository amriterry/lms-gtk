#include <gtk/gtk.h>

#include <system/database/connections/statements/SQLiteStatement.h>
#include <system/database/Binding.h>

#include <system/facades/Facades.h>

namespace tuber{

SQLiteStatement::SQLiteStatement(sqlite3* connection) : m_connection(connection){}

sqlite3_stmt* SQLiteStatement::getRawStatement(){
	return this->m_stmt;
}

int SQLiteStatement::getAffectedId(){
	sqlite3_int64 val = sqlite3_last_insert_rowid(this->m_connection);
	g_message("SQLiteStatement: Last Insert Id: %i",val);
	return val;
}

int SQLiteStatement::getAffectedNum(){
	return sqlite3_changes(this->m_connection);
}

Statement* SQLiteStatement::prepare(string query){
    //g_message("SQLiteStatement: Preparing query");
	int rc = sqlite3_prepare_v2(this->m_connection,query.c_str(),-1,&m_stmt,NULL);
	if(rc != SQLITE_OK){
		g_error("Prepare Query Error: %s",sqlite3_errmsg(this->m_connection));
	}
	return this;
}

Statement* SQLiteStatement::bindData(Binding* bindings){
	//g_message("SQLiteStatement: Binding data");
	if(bindings != nullptr){
		vector<BindingInfo> bindingInfo = bindings->getParsedBindingMeta();
		int count = 1;
		for(auto& it: bindingInfo){
		    int rc;
		    string str;
			switch(it.dataType){
				case BindingType::INTEGER:
					g_message("SQLiteStatement: Binding int %i at pos %i",bindings->getIntegerAt(it.bindingPos),it.bindingPos);
					rc = sqlite3_bind_int64(this->getRawStatement(),count,bindings->getIntegerAt(it.bindingPos));
					if(SQLITE_OK != rc){
						g_error("Binding Query Error:[int] %s",sqlite3_errmsg(this->m_connection));
					}
					count++;
					break;
				case BindingType::DOUBLE:
					g_message("SQLiteStatement: Binding double [%f] at pos %i",bindings->getDoubleAt(it.bindingPos),it.bindingPos);
					rc = sqlite3_bind_double(this->getRawStatement(),count,bindings->getDoubleAt(it.bindingPos));
					if(SQLITE_OK != rc){
						g_error("Binding Query Error:[double] %s",sqlite3_errmsg(this->m_connection));
					}
					count++;
					break;
				case BindingType::STRING:
					str = bindings->getStringAt(it.bindingPos);
					g_message("SQLiteStatement: Binding string [%s] at pos %i",bindings->getStringAt(it.bindingPos).c_str(),it.bindingPos);
					rc = sqlite3_bind_text(this->getRawStatement(),count,str.c_str(),str.length(),NULL);
					if(SQLITE_OK != rc){
						g_error("Binding Query Error:[string] %s",sqlite3_errmsg(this->m_connection));
					}
					count++;
					break;
				case BindingType::BOOLEAN:
					g_message("SQLiteStatement: Binding bool [%i] at pos %i",bindings->getBooleanAt(it.bindingPos),it.bindingPos);
					rc = sqlite3_bind_int64(this->getRawStatement(),count,(bindings->getBooleanAt(it.bindingPos) == true)?1:0);
					if(SQLITE_OK != rc){
						g_error("Binding Query Error:[bool] %s",sqlite3_errmsg(this->m_connection));
					}
					count++;
					break;
				case BindingType::GENERIC:
					g_message("SQLiteStatement: Binding null at pos %i",it.bindingPos);
					rc = sqlite3_bind_null(this->getRawStatement(),count);
					if(SQLITE_OK != rc){
						g_error("Binding Query Error:[null] %s",sqlite3_errmsg(this->m_connection));
					}
					count++;
					break;
				default:
					break;
			}
		}
	}

	/*if(bindings != nullptr){
		if(bindings->hasNull()){
            vector<int> nulls = bindings->getNull();

			for (vector<int>::iterator i = nulls.begin(); i != nulls.end(); ++i){
				int rc = sqlite3_bind_null(this->getRawStatement(),*i);
				g
				if(SQLITE_OK != rc){
					g_error("Query Error: %s",sqlite3_errmsg(this->m_connection));
				}
			}
		}

		if(bindings->hasIntegers()){
            map<int,int> integers = bindings->getIntegers();

			for(map<int,int>::iterator itr = integers.begin(); itr != integers.end(); ++itr){
				int rc = sqlite3_bind_int64(this->getRawStatement(),itr->first,itr->second);
				g_message("SQLiteStatement: Binding int %i at pos %i",itr->second,itr->first);
				if(SQLITE_OK != rc){
					g_error("Query Error: %s",sqlite3_errmsg(this->m_connection));
				}
			}
		}

		if(bindings->hasStrings()){
            map<int,string> strings = bindings->getStrings();

			for(map<int,string>::iterator itr = strings.begin(); itr != strings.end(); ++itr){
				int rc = sqlite3_bind_text(this->getRawStatement(),itr->first,itr->second.c_str(),itr->second.length(),NULL);
				g_message("SQLiteStatement: Binding string [%s] at pos %i",itr->second.c_str(),itr->first);
				if(SQLITE_OK != rc){
					g_error("Query Error: %s",sqlite3_errmsg(this->m_connection));
				}
			}
		}

		if(bindings->hasDoubles()){
            map<int,double> doubles = bindings->getDoubles();

			for(map<int,double>::iterator itr = doubles.begin(); itr != doubles.end(); ++itr){
				int rc = sqlite3_bind_double(this->getRawStatement(),itr->first,itr->second);
				g_message("SQLiteStatement: Binding double [%f] at pos %i",itr->second,itr->first);
				if(SQLITE_OK != rc){
					g_error("Query Error: %s",sqlite3_errmsg(this->m_connection));
				}
			}
		}

		if(bindings->hasBooleans()){
            map<int,bool> booleans = bindings->getBooleans();

			for(map<int,bool>::iterator itr = booleans.begin(); itr != booleans.end(); ++itr){
				int rc = sqlite3_bind_int(this->getRawStatement(),itr->first,(itr->second == true)?1:0);
				g_message("SQLiteStatement: Binding bool [%i] at pos %i",itr->second,itr->first);
				if(SQLITE_OK != rc){
					g_error("Query Error: %s",sqlite3_errmsg(this->m_connection));
				}
			}
		}
		App::get()->destroyBindable(bindings);
	}*/
	return this;
}

Statement* SQLiteStatement::step(Binding* bindings){
	this->bindData(bindings);

	//g_message("SQLiteStatement: Query executed: \n\n[%s]\n\n",sqlite3_sql(this->getRawStatement()));

	int rc = sqlite3_step(this->getRawStatement());

	//g_message("SQLiteStatement: Return Code [%i]",rc);

	if(rc != SQLITE_DONE){
		g_error("Query Error: code[%i] - %s",sqlite3_errcode(this->m_connection),sqlite3_errmsg(this->m_connection));
	}

	//g_message("SQLiteStatement: Statement stepped");

	return this;
}

Statement* SQLiteStatement::execute(string query,Binding* bindings){
	return this->prepare(query)->step(bindings);
}

QueryResult SQLiteStatement::executeAndFetch(string query,Binding* bindings){
	QueryResult result;
    //g_message("SQLiteStatement: Preparing query and Binding data");
	this->prepare(query)->bindData(bindings);
	//g_message("SQLiteStatement: Query [\n\n%s\n\n]",query.c_str());
    //g_message("SQLiteStatement: Prepared query and Binding data");
	int rc;

	while(SQLITE_ROW == (rc = sqlite3_step(m_stmt))) {
		QueryRow row = BundleFacade::get();
        //g_message("SQLiteStatement: Got A row");
        //QueryRow row;
		//int col;
		for(int col=0; col<sqlite3_column_count(m_stmt); col++) {
		    string key = (const char*) sqlite3_column_name(m_stmt,col);
		    //g_message("SQLiteStatement: Putting row value in Bundle for key [%s] in ",key.c_str());
		    string value;
		    switch(sqlite3_column_type(m_stmt,col)){
		    	case SQLITE_INTEGER:
		    		g_message("int");
		    		row->putExtra(key,(int) sqlite3_column_int64(m_stmt,col));
		    		break;
		    	case SQLITE_FLOAT:
		    		g_message("double");
		    		row->putExtra(key,sqlite3_column_double(m_stmt,col));
		    		break;
		    	case SQLITE3_TEXT:
		    		g_message("string");
		    		row->putExtra(key,string((const char*) sqlite3_column_text(m_stmt,col)));
		    		break;
		    	case SQLITE_NULL:
		    		g_message("null");
		    		row->putExtra(key,string(""));
		    		break;
		    	default:
		    		break;
		    }
		}
		this->m_result.push_back(row);
	}

	if(rc != SQLITE_DONE){
		g_error("Select Query Error: %s",sqlite3_errmsg(this->m_connection));
	}

	if(this->m_result.empty()){
        this->m_result.push_back(BundleFacade::get());
	}

    g_message("SQLiteStatement: Returning Result");
	return this->m_result;
}

int SQLiteStatement::getColumnCount(){
	return sqlite3_column_count(m_stmt);
}

map<string,int> SQLiteStatement::getColumnNames(){
	map<string,int> names;
	for(int i = 0;i < getColumnCount();i++){
		names[string(sqlite3_column_name(m_stmt,i))] = i;
	}
	return names;
}

int SQLiteStatement::getColumnPos(string columnName){
	return (getColumnNames())[columnName];
}

void SQLiteStatement::executeWithCallback(string query,Binding* bindings,RowCallback callback,Bundle* data){
	this->prepare(query)->bindData(bindings);
	int rc;
	map<string,int> columns;
	while(SQLITE_ROW == (rc = sqlite3_step(m_stmt))) {
		if(columns.empty()){
			columns = getColumnNames();
		}
		callback(this,data);
	}

	if(rc != SQLITE_DONE){
		g_error("Select Query Error: %s",sqlite3_errmsg(this->m_connection));
	}
}

int SQLiteStatement::getInt(string columnName){
	return sqlite3_column_int64(m_stmt,getColumnPos(columnName));
}

double SQLiteStatement::getDouble(string columnName){
	return sqlite3_column_double(m_stmt,getColumnPos(columnName));
}

string SQLiteStatement::getString(string columnName){
	const char* str = (const char*) sqlite3_column_text(m_stmt,getColumnPos(columnName));
	if(str == NULL){
		str = "";
	}
	return string(str);
}

SQLiteStatement::~SQLiteStatement(){
	sqlite3_finalize(this->m_stmt);
}


}
