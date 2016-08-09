#ifndef _MODEL_QUERY_BUILDER_H_
#define _MODEL_QUERY_BUILDER_H_

#include <vector>
#include <string>
#include <system/database/connections/QueryResult.h>
#include <system/database/query/RowCallback.h>

using namespace std;

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
	QueryResult get(vector<string> selectStatements);
	void get(vector<string> selectStatements,RowCallback callback,Bundle* data);
	int insert(Bundle* attributes);
	bool update(Bundle* attributes);
	bool deleteModel();
private:
	QueryBuilder* m_queryBuilder;
	Model* m_model;
	bool m_isModelBinded;
};

}

#endif
