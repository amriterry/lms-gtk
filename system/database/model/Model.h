#ifndef _MODEL_H_
#define _MODEL_H_

#include <string>

#include <system/support/IEventPublisher.h>
#include <system/database/connections/QueryResult.h>
#include <system/database/model/ModelQueryBuilder.h>

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

	template<typename T>
	static T* find(int primaryKey){
		T* model = new T();
		return dynamic_cast<T*>(model->newQuery()->find(primaryKey));
	}

	template<typename T>
	static QueryResult all(vector<string> selectStatements){
		T* model = new T();
		return model->newQuery()->get(selectStatements);
	}

	bool saveRow();
	bool deleteRow();
	QueryRow fetch();
	Model* fill(QueryRow row);
	Model* syncOriginal();
	Model* moveToOriginal();

	int getPrimaryKeyValue();

	QueryRow getDirty();

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

	void finishSave();
	void finishDelete();
	bool performUpdate(ModelQueryBuilder* query);
	bool performInsert(ModelQueryBuilder* query);

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
