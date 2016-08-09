#ifndef _MODEL_H_
#define _MODEL_H_

#include <string>

#include <system/support/IEventPublisher.h>
#include <system/database/connections/QueryResult.h>
#include <system/database/query/QueryBuilder.h>
#include <system/database/model/ModelQueryBuilder.h>
#include <system/database/connections/statements/Statement.h>

#include <system/helpers/helpers.h>

using namespace std;

namespace tuber{

class IConnectionResolver;
class ModelQueryBuilder;
class EventDispatcher;
class QueryBuilder;
class Connection;

class Model: public IEventPublisher{
public:
	Model();
	static void setEventDispatcher(EventDispatcher* dispatcher);
	static void setConnectionResolver(IConnectionResolver* resolver);

	static QueryBuilder* select(vector<string> columns);

	static QueryBuilder* table(string table);

	template<typename T>
	static T* find(int primaryKey){
		T* model = new T();
		return dynamic_cast<T*>(model->newQuery()->find(primaryKey));
	}

	template<typename T>
	static QueryResult all(vector<string> selectStatements = {"*"}){
		T* model = new T();
		QueryResult result = model->newQuery()->get(selectStatements);
		delete model;
		return result;
	}

	template<typename T>
	static void all(RowCallback callback,Bundle* data = nullptr,vector<string> selectStatements = {"*"}){
		T* model = new T();
		model->newQuery()->get(selectStatements,callback,data);
		delete model;
	}

	bool exists();
	bool saveRow();
	bool deleteRow();
	QueryRow fetch();
	Model* fill(QueryRow row);
	Model* syncOriginal();
	Model* moveToOriginal();

	int getPrimaryKeyValue();

	QueryRow getDirty();

	int getKey();
	virtual string getKeyName();
	virtual bool usesTimestamps();
	virtual string getTableName();

	virtual void setModelAttributes(string table,string primaryKey = "id",bool timestamps = "true");
protected:
	bool m_exists;
	QueryRow m_original;
	QueryRow m_attributes;

	string m_table = "";
	bool m_timestamps = true;
	string m_primaryKey = "";

	const string CREATED_AT = "created_at";
	const string UPDATED_AT = "updated_at";

	static IConnectionResolver* m_resolver;
	static EventDispatcher* m_dispatcher;

	ModelQueryBuilder* newQuery();

	void finishSave(int savedId = 0);
	void finishDelete();
	bool performUpdate(ModelQueryBuilder* query);
	int performInsert(ModelQueryBuilder* query);

	bool isDirty();

	string getFreshTime();
	void updateTimestamps();
	void setCreatedAt(string timestamp);
	void setUpdatedAt(string timestamp);

	Bundle* getOriginalBundle();
	Bundle* getAttributesBundle();

	Connection* getConnection(string connection = "");

	static Connection* resolveConnection(string connection);
};

}

#endif
