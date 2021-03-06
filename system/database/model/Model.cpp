#include <cstdio>
#include <ctime>

#include <gtk/gtk.h>

#include <vendor/sqlite/sqlite3.h>

#include <system/database/model/Model.h>

#include <system/database/IConnectionResolver.h>
#include <system/database/connections/Connection.h>

#include <system/database/query/QueryBuilder.h>
#include <system/database/model/ModelQueryBuilder.h>

#include <system/facades/Facades.h>

#include <system/foundation/RunTimeException.h>

namespace tuber{

IConnectionResolver* Model::m_resolver = nullptr;
EventDispatcher* Model::m_dispatcher = nullptr;

Model::Model(){
	this->m_original = BundleFacade::get();
	this->m_attributes = BundleFacade::get();
	this->m_exists = false;
}

void Model::setConnectionResolver(IConnectionResolver* resolver){
	m_resolver = resolver;
}

void Model::setEventDispatcher(EventDispatcher* dispatcher){
	m_dispatcher = dispatcher;
}

ModelQueryBuilder* Model::newQuery(){
	Connection* conn = this->getConnection();
	ModelQueryBuilder* builder = new ModelQueryBuilder(new QueryBuilder(conn,conn->getQueryGrammar()));
	return builder->bindModel(this);
}

Connection* Model::getConnection(string connection){
	return resolveConnection(connection);
}

Connection* Model::resolveConnection(string connection){
	return m_resolver->connection(connection);
}

string Model::getTableName(){
	return this->m_table;
}

int Model::getKey(){
	if(this->m_original != nullptr && this->m_original->hasInt(this->getKeyName())){
		return this->m_original->getInt(this->getKeyName());
	}
	if(this->m_attributes != nullptr && this->m_attributes->hasInt(this->getKeyName())){
		return this->m_attributes->getInt(this->getKeyName());
	}
	return 0;
}

string Model::getKeyName(){
	return this->m_primaryKey;
}

void Model::setModelAttributes(string table,string primaryKey,bool timestamps){
    this->m_table = table;
    this->m_primaryKey = primaryKey;
    this->m_timestamps = timestamps;
}

bool Model::usesTimestamps(){
	return this->m_timestamps;
}

string Model::getFreshTime(){
	return DateTime::getCurrentDateTime();
}

void Model::setCreatedAt(string timestamp){
	this->m_attributes->putExtra(CREATED_AT,timestamp);
}

void Model::setUpdatedAt(string timestamp){
	this->m_attributes->putExtra(UPDATED_AT,timestamp);
}

void Model::updateTimestamps(){
	string timestr = this->getFreshTime();
	if(this->m_exists){
		this->setUpdatedAt(timestr);
	} else {
		this->setCreatedAt(timestr);
		this->setUpdatedAt(timestr);
	}

}

int Model::getPrimaryKeyValue(){
	if(this->m_exists){
		return this->m_original->getInt(this->getKeyName());
	}
	return 0;
}

Model* Model::fill(QueryRow row){
	g_message("Model: Filling Query Row");
	if(row->empty()){
		g_message("Model: Not empty row");
	}
	this->m_attributes->clone(row);
	g_message("Model: Row copied");
	if(this->m_attributes->hasInt(this->getKeyName())){
		g_message("Model: Has Primary Key");
		this->m_exists = true;
	} else {
		g_message("Model: Does not have Primary Key");
		this->m_exists = false;
	}
	g_message("Model: Row Filled");
	return this;
}

Model* Model::syncOriginal(){
	this->m_original->clone(this->m_attributes);
	return this;
}

Model* Model::moveToOriginal(){
	g_message("Model: Moving attributes to original");
	this->m_original->clone(this->m_attributes);
	g_message("Model: Reseting attributes");
	this->m_attributes->reset();
	return this;
}

QueryRow Model::fetch(){
	return this->m_original;
}

QueryBuilder* Model::select(vector<string> columns){
	Model* model = new Model();
	QueryBuilder* builder = model->newQuery()->getQueryBuilder()->select(columns);
	delete model;
	return builder;
}

QueryBuilder* Model::table(string table){
	Model* model = new Model();
	QueryBuilder* builder = model->newQuery()->getQueryBuilder()->table(table);
	delete model;
	return builder;
}

bool Model::saveRow(){
	ModelQueryBuilder* query = this->newQuery();
	int savedId;
	bool saved = false;
	if(this->m_exists){
		//...found primary key, so update
		savedId = this->fetch()->getInt(this->getKeyName());
		saved = this->performUpdate(query);

	} else {
		//...primary key not found so insert
		savedId = this->performInsert(query);
		if(savedId != 0){
			saved = true;
		}
	}
	if(saved){
		this->finishSave(savedId);
	}
	return saved;
}

bool Model::deleteRow(){
	if(this->m_primaryKey == ""){
		throw RunTimeException("No primary key defined on the model");
	}
	if(this->m_exists){
		g_message("Model: attributes are loaded so it is safe to delete model");
        bool result = this->newQuery()->deleteModel();
        if(result){
        	this->finishDelete();
        }
        return result;
	}
	return false;
}

bool Model::isDirty(){
	return !this->m_attributes->empty();
}

QueryRow Model::getDirty(){
	return this->m_attributes;
}

void Model::finishSave(int savedId){
	if(savedId != 0){
		g_message("Saved row id: %i",savedId);
		this->getDirty()->putExtra(this->getKeyName(),savedId);
	}
	this->moveToOriginal();
}

void Model::finishDelete(){
	this->m_original->reset();
	this->m_attributes->reset();
}

bool Model::performUpdate(ModelQueryBuilder* query){
	if(this->usesTimestamps()){
		this->updateTimestamps();
	}

	return query->update(this->getDirty());
}

int Model::performInsert(ModelQueryBuilder* query){
	g_message("Model: inserting to database");
	if(this->usesTimestamps()){
		this->updateTimestamps();
	}

	return query->insert(this->getDirty());
}

Bundle* Model::getOriginalBundle(){
	return this->m_original;
}

Bundle* Model::getAttributesBundle(){
	return this->m_attributes;
}

bool Model::exists(){
	return m_exists;
}

}
