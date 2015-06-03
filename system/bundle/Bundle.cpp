#include <system/bundle/Bundle.h>
#include <system/foundation/KeyNotFoundException.h>

#include <string>
#include <map>

using namespace std;

namespace tuber{

Bundle* Bundle::putExtra(string key,int value){
	this->m_integers[key] = value;
	return this;
}

Bundle* Bundle::putExtra(string key,double value){
    this->m_doubles[key] = value;
    return this;
}

Bundle* Bundle::putExtra(string key,bool value){
    this->m_booleans[key] = value;
    return this;
}

Bundle* Bundle::putExtra(string key,string value){
    this->m_strings[key] = value;
    return this;
}

Bundle* Bundle::putExtra(string key,void* value){
    this->m_generics[key] = value;
    return this;
}

int Bundle::getInt(string key){
    if(this->m_integers.find(key) != this->m_integers.end()){
        return this->m_integers[key];
    } else {
        throw KeyNotFoundException(key);
    }
}

double Bundle::getDouble(string key){
    if(this->m_doubles.find(key) != this->m_doubles.end()){
        return this->m_doubles[key];
    } else {
        throw KeyNotFoundException(key);
    }
}

bool Bundle::getBoolean(string key){
    if(this->m_booleans.find(key) != this->m_booleans.end()){
        return this->m_booleans[key];
    } else {
        throw KeyNotFoundException(key);
    }
}

string Bundle::getString(string key){
    if(this->m_strings.find(key) != this->m_strings.end()){
        return this->m_strings[key];
    } else {
        throw KeyNotFoundException(key);
    }
}


void Bundle::clear(){

}

}
