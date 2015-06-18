#ifndef _BUNDLE_H_
#define _BUNDLE_H_

#include <string>
#include <map>

#include <system/foundation/Service.h>
#include <system/foundation/KeyNotFoundException.h>
#include <system/bundle/BindingType.h>

using namespace std;

namespace tuber{

class Bundle: public Service{
public:
	Bundle();
	~Bundle();
	static constexpr int DEFAULT_INT = 0;
	static constexpr double DEFAULT_DOUBLE = 0.0;
	static constexpr bool DEFAULT_BOOLEAN = false;
	static constexpr char* DEFAULT_STRING = "";

	Bundle* putExtra(string key,int value);
	Bundle* putExtra(string key,bool value);
	Bundle* putExtra(string key,double value);
	Bundle* putExtra(string key,string value);
	Bundle* putExtra(string key,const char* value);
	Bundle* putExtra(string key,void* value);

	map<string,int>& getIntMap();
	map<string,bool>& getBooleanMap();
	map<string,double>& getDoubleMap();
	map<string,string>& getStringMap();
	map<string,void*>& getGenericMap();

	bool hasInt(string key);
	bool hasDouble(string key);
	bool hasBoolean(string key);
	bool hasString(string key);
	bool hasGeneric(string key);

	int getInt(string key);
	double getDouble(string key);
	bool getBoolean(string key);
	string getString(string key);

	template <class T>
	T* getGeneric(string key){
	    if(hasGeneric(key)){
	        return static_cast<T*>(this->m_generics[key]);
	    } else {
	        throw KeyNotFoundException(key);
	    }
	}

	int getSafeInt(string key);
	bool getSafeBoolean(string key);
	double getSafeDouble(string key);
	string getSafeString(string key);

	template <class T>
	T* getSafeGeneric(string key){
	    if(hasGeneric(key)){
	        return static_cast<T*>(this->m_generics[key]);
	    } else {
	        return nullptr;
	    }
	}

	void reset();
	bool empty();
	void move(Bundle* toMoveBundle);
	void clone(Bundle* toCloneBundle);

	map<int,pair<string,BindingType>>& getBindingPositionMap();
	void setBindingPositionMap(map<int,pair<string,BindingType>> bindingPos);

	pair<string,BindingType>& getPairAtPos(int pos);

	string getKeyAtPos(int pos);

	int getIntAtPos(int pos);
	double getDoubleAtPos(int pos);
	bool getBooleanAtPos(int pos);
	string getStringAtPos(int pos);

	template <class T>
	T* getGenericAtPos(int pos){
		if(pos > 0 && pos <= currentPos){
			pair<string,BindingType> bindingMeta = this->m_bindingPos[pos];
			if(bindingMeta.second != BindingType::NULLTYPE){
				//...throw an exception
			}
			return getGeneric<T>(bindingMeta.first);
		} else {
			//...throw an out of bounds exception
		}
	}
private:
	int currentPos = 1;
	map<string,int> m_integers;
	map<string,double> m_doubles;
	map<string,bool> m_booleans;
	map<string,string> m_strings;
	map<string,void*> m_generics;
	map<int,pair<string,BindingType>> m_bindingPos; 
};

}

#endif
