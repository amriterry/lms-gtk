#include <gtk/gtk.h>

#include <system/database/model/ModelQueryBuilder.h>

#include <system/database/connections/QueryResult.h>
#include <system/database/model/Model.h>
#include <system/database/query/QueryBuilder.h>

namespace tuber{

ModelQueryBuilder::ModelQueryBuilder(QueryBuilder* builder) : m_queryBuilder(builder){
	this->m_isModelBinded = false;
}

ModelQueryBuilder::~ModelQueryBuilder(){
	delete this->m_queryBuilder;
}

ModelQueryBuilder* ModelQueryBuilder::bindModel(Model* model){
	this->m_model = model;
	this->m_isModelBinded = true;
	return this;
}

QueryBuilder* ModelQueryBuilder::getQueryBuilder(){
	return this->m_queryBuilder;
}

void ModelQueryBuilder::setQueryBuilder(QueryBuilder* builder){
	this->m_queryBuilder = builder;
}

Model* ModelQueryBuilder::find(int pk){
	QueryRow result = this->m_queryBuilder->select({"*"})
				->from(this->m_model->getTableName())
				->where(this->m_model->getKeyName(),"=",pk)
				->first();
	this->m_model->fill(result)->moveToOriginal();
	return this->m_model;
}

bool ModelQueryBuilder::insert(Bundle* attributes){
	return this->m_queryBuilder->table(this->m_model->getTableName())
								->insert(attributes);
}

bool ModelQueryBuilder::update(Bundle* attributes){
	return this->m_queryBuilder->table(this->m_model->getTableName())
								->where(this->m_model->getKeyName(),"=",this->m_model->getPrimaryKeyValue())
								->update(attributes);
}

bool ModelQueryBuilder::deleteModel(){
	g_message("PK = %i ",this->m_model->getPrimaryKeyValue());
	return this->m_queryBuilder->table(this->m_model->getTableName())
						->where(this->m_model->getKeyName(),"=",this->m_model->getPrimaryKeyValue())
						->deleteData();
}


}
