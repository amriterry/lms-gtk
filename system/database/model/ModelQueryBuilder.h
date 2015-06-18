#ifndef _MODEL_QUERY_BUILDER_H_
#define _MODEL_QUERY_BUILDER_H_

namespace tuber{

class Model;
class Bundle;
class QueryBuilder;

class ModelQueryBuilder{
public:
	ModelQueryBuilder(QueryBuilder* builder);
	virtual ~ModelQueryBuilder();

	ModelQueryBuilder* bindModel(Model* model);

	QueryBuilder* getQueryBuilder();
	void setQueryBuilder(QueryBuilder* builder);

	Model* find(int pk);
	bool insert(Bundle* attributes);
	bool update(Bundle* attributes);
	bool deleteModel();
private:
	QueryBuilder* m_queryBuilder;
	Model* m_model;
	bool m_isModelBinded;
};

}

#endif
