#ifndef _BUNDLE_H_
#define _BUNDLE_H_

#include <string>
#include <map>

#include <system/foundation/Service.h>
#include <system/foundation/KeyNotFoundException.h>

using namespace std;

namespace tuber{

class Bundle: public Service{
public:
	Bundle* putExtra(string key,int value);
	Bundle* putExtra(string key,double value);
	Bundle* putExtra(string key,bool value);
	Bundle* putExtra(string key,string value);
	Bundle* putExtra(string key,void* value);

	int getInt(string key);
	double getDouble(string key);
	bool getBoolean(string key);
	string getString(string key);

	template <class T>
	T* getGeneric(string key){
	    if(this->m_generics.find(key) != this->m_generics.end()){
	        return static_cast<T*>(this->m_generics[key]);
	    } else {
	        throw KeyNotFoundException(key);
	    }
	}


	void clear();
private:
	map<string,int> m_integers;
	map<string,double> m_doubles;
	map<string,bool> m_booleans;
	map<string,string> m_strings;
	map<string,void*> m_generics;
};

}

#endif
