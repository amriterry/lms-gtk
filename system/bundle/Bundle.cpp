#include <system/bundle/Bundle.h>
#include <system/foundation/KeyNotFoundException.h>

#include <system/facades/Facades.h>

#include <string>
#include <map>

using namespace std;

namespace tuber{

Bundle::Bundle(){
    g_message("Bundle: object created");
}

Bundle::~Bundle(){
    g_message("Bundle: object destroyed");
}

/**
 * @brief Puts integer into the basket
 * 
 * @param key Key with which the value is to be bound
 * @param value Value for the key
 * 
 * @return Same object for function call chaining
 */
Bundle* Bundle::putExtra(string key,int value){
	this->m_integers[key] = value;
    this->m_bindingPos[currentPos] = pair<string,BindingType>(key,BindingType::INTEGER);
    currentPos++;
	return this;
}

/**
 * @brief Puts double into the basket
 * 
 * @param key Key with which the value is to be bound
 * @param value Value for the key
 * 
 * @return Same object for function call chaining
 */
Bundle* Bundle::putExtra(string key,double value){
    this->m_doubles[key] = value;
    this->m_bindingPos[currentPos] = pair<string,BindingType>(key,BindingType::DOUBLE);
    currentPos++;
    return this;
}

/**
 * @brief Puts bool into the basket
 * 
 * @param key Key with which the value is to be bound
 * @param value Value for the key
 * 
 * @return Same object for function call chaining
 */
Bundle* Bundle::putExtra(string key,bool value){
    this->m_booleans[key] = value;
    this->m_bindingPos[currentPos] = pair<string,BindingType>(key,BindingType::BOOLEAN);
    currentPos++;
    return this;
}

/**
 * @brief Puts string into the basket
 * 
 * @param key Key with which the value is to be bound
 * @param value Value for the key
 * 
 * @return Same object for function call chaining
 */
Bundle* Bundle::putExtra(string key,string value){
    this->m_strings[key] = value;
    this->m_bindingPos[currentPos] = pair<string,BindingType>(key,BindingType::STRING);
    currentPos++;
    return this;
}

/**
 * @brief Puts char[] into the basket
 * 
 * @param key Key with which the value is to be bound
 * @param value Value for the key
 * 
 * @return Same object for function call chaining
 */
Bundle* Bundle::putExtra(string key,const char* value){
    return this->putExtra(key,string(value));
}

/**
 * @brief Puts generic value into the basket
 * 
 * @param key Key with which the value is to be bound
 * @param value Value for the key
 * 
 * @return Same object for function call chaining
 */
Bundle* Bundle::putExtra(string key,void* value){
    this->m_generics[key] = value;
    this->m_bindingPos[currentPos] = pair<string,BindingType>(key,BindingType::GENERIC);
    currentPos++;
    return this;
}

/**
 * @brief Checks if there is integer into the basket
 * 
 * @param key Key for which the integer is to be checked for
 * @return bool
 */
bool Bundle::hasInt(string key){
    return (this->m_integers.find(key) != this->m_integers.end());
}

/**
 * @brief Checks if there is double into the basket
 * 
 * @param key Key for which the double is to be checked for
 * @return bool
 */
bool Bundle::hasDouble(string key){
    return (this->m_doubles.find(key) != this->m_doubles.end());
}

/**
 * @brief Checks if there is boolean into the basket
 * 
 * @param key Key for which the boolean is to be checked for
 * @return bool
 */
bool Bundle::hasBoolean(string key){
    return (this->m_booleans.find(key) != this->m_booleans.end());
}

/**
 * @brief Checks if there is string into the basket
 * 
 * @param key Key for which the string is to be checked for
 * @return bool
 */
bool Bundle::hasString(string key){
    return (this->m_strings.find(key) != this->m_strings.end());
}

/**
 * @brief Checks if there is generic value into the basket
 * 
 * @param key Key for which the generic value is to be checked for
 * @return bool
 */
bool Bundle::hasGeneric(string key){
    return (this->m_generics.find(key) != this->m_generics.end());
}

/**
 * @brief Getter for a integer value in the package
 * 
 * @param key Key for which value is to be found
 * @return Value for the key
 */
int Bundle::getInt(string key){
    if(hasInt(key)){
        return this->m_integers[key];
    } else {
        throw KeyNotFoundException(key);
    }
}

/**
 * @brief Getter for a double value in the package
 * 
 * @param key Key for which value is to be found
 * @return Value for the key
 */
double Bundle::getDouble(string key){
    if(hasDouble(key)){
        return this->m_doubles[key];
    } else {
        throw KeyNotFoundException(key);
    }
}

/**
 * @brief Getter for a boolean value in the package
 * 
 * @param key Key for which value is to be found
 * @return Value for the key
 */
bool Bundle::getBoolean(string key){
    if(hasBoolean(key)){
        return this->m_booleans[key];
    } else {
        throw KeyNotFoundException(key);
    }
}

/**
 * @brief Getter for a string value in the package
 * 
 * @param key Key for which value is to be found
 * @return Value for the key
 */
string Bundle::getString(string key){
    if(hasString(key)){
        return this->m_strings[key];
    } else {
        throw KeyNotFoundException(key);
    }
}

/**
 * @brief Getter for a integer value in the package if exists else returns default value
 * 
 * @param key Key for which value is to be found
 * @return Value for the key
 */
int Bundle::getSafeInt(string key){
    if(hasInt(key)){
        return this->m_integers[key];
    } else {
        return Bundle::DEFAULT_INT;
    }
}

/**
 * @brief Getter for a boolean value in the package if exists else returns default value
 * 
 * @param key Key for which value is to be found
 * @return Value for the key
 */
bool Bundle::getSafeBoolean(string key){
    if(hasBoolean(key)){
        return this->m_booleans[key];
    } else {
        return Bundle::DEFAULT_BOOLEAN;
    }
}

/**
 * @brief Getter for a double value in the package if exists else returns default value
 * 
 * @param key Key for which value is to be found
 * @return Value for the key
 */
double Bundle::getSafeDouble(string key){
    if(hasDouble(key)){
        return this->m_doubles[key];
    } else {
        return Bundle::DEFAULT_DOUBLE;
    }
}

/**
 * @brief Getter for a string value in the package if exists else returns default value
 * 
 * @param key Key for which value is to be found
 * @return Value for the key
 */
string Bundle::getSafeString(string key){
    if(hasString(key)){
        return this->m_strings[key];
    } else {
        return Bundle::DEFAULT_STRING;
    }
}

/**
 * @brief Getter for the integer map
 * @return Refrence to the map
 */
map<string,int>& Bundle::getIntMap(){
    return this->m_integers;
}

/**
 * @brief Getter for the double map
 * @return Refrence to the map
 */
map<string,double>& Bundle::getDoubleMap(){
    return this->m_doubles;
}

/**
 * @brief Getter for the boolean map
 * @return Refrence to the map
 */
map<string,bool>& Bundle::getBooleanMap(){
    return this->m_booleans;
}

/**
 * @brief Getter for the strings map
 * @return Refrence to the map
 */
map<string,string>& Bundle::getStringMap(){
    return this->m_strings;
}

/**
 * @brief Getter for the generic map
 * @return Refrence to the map
 */
map<string,void*>& Bundle::getGenericMap(){
    return this->m_generics;
}

/**
 * @brief Clones value of one Bundle to this bundle
 * 
 * @param toCloneBundle Bundle to be cloned
 */
void Bundle::clone(Bundle* toCloneBundle){
    this->m_integers = toCloneBundle->getIntMap();
    this->m_doubles = toCloneBundle->getDoubleMap();
    this->m_strings = toCloneBundle->getStringMap();
    this->m_booleans = toCloneBundle->getBooleanMap();
    this->m_generics = toCloneBundle->getGenericMap();
    this->m_bindingPos = toCloneBundle->getBindingPositionMap();
}

/**
 * @brief Clones and destroys the previous bundle
 * 
 * @param toMoveBundle Bundle to be moved
 */
void Bundle::move(Bundle* toMoveBundle){
    this->clone(toMoveBundle);
    App::destroy(toMoveBundle);
}

/**
 * @brief Resets the value of each map and current position
 */
void Bundle::reset(){
    this->m_integers.clear();
    this->m_doubles.clear();
    this->m_strings.clear();
    this->m_booleans.clear();
    this->m_generics.clear();
    this->m_bindingPos.clear();
    this->currentPos = 1;
}

/**
 * @brief Checks if the bundle is empty
 * @return bool
 */
bool Bundle::empty(){
    //  Checking if all the maps are empty
    return (m_integers.empty() && m_doubles.empty() && m_strings.empty() && m_booleans.empty() && m_generics.empty());
}

/**
 * @brief Returns the binding position map
 * @return Refrence to the map
 */
map<int,pair<string,BindingType>>& Bundle::getBindingPositionMap(){
    return this->m_bindingPos;
}

/**
 * @brief Setter for the Binding position map
 * 
 * @param bindingPos Map to be set as binding position map
 */
void Bundle::setBindingPositionMap(map<int,pair<string,BindingType>> bindingPos){
    this->m_bindingPos = bindingPos;
}

/**
 * @brief Getter for the pair at a certain position
 * 
 * @param pos Position to be searched for
 * @return Pair at given position
 */
pair<string,BindingType>& Bundle::getPairAtPos(int pos){
    return this->m_bindingPos[pos];
}

/**
 * @brief Returns the key of object at the given position
 * 
 * @param pos Position at which the key is to be found
 * @return Key at the position
 * 
 * @todo Throw an OutOfBoundsException
 */
string Bundle::getKeyAtPos(int pos){
    if(pos > 0 && pos <= currentPos){
        return this->m_bindingPos[pos].first;
    } else {
        //...throw an out of bounds exception
    }
}

/**
 * @brief Returns integer at the given position
 * 
 * @param pos Position at which the integer is to be searched
 * @return Ingteger at the position
 * 
 * @todo Throw Exceptions
 */
int Bundle::getIntAtPos(int pos){
    if(pos > 0 && pos <= currentPos){
        pair<string,BindingType> bindingMeta = this->m_bindingPos[pos];
        if(bindingMeta.second != BindingType::INTEGER){
            //...throw an exception
        }
        return getInt(bindingMeta.first);
    } else {
        //...throw an out of bounds exception
    }
}

/**
 * @brief Returns double at the given position
 * 
 * @param pos Position at which the double is to be searched
 * @return Double at the position
 * 
 * @todo Throw Exceptions
 */
double Bundle::getDoubleAtPos(int pos){
    if(pos > 0 && pos <= currentPos){
        pair<string,BindingType> bindingMeta = this->m_bindingPos[pos];
        if(bindingMeta.second != BindingType::DOUBLE){
            //...throw an exception
        }
        return getDouble(bindingMeta.first);
    } else {
        //...throw an out of bounds exception
    }
}

/**
 * @brief Returns boolean at the given position
 * 
 * @param pos Position at which the boolean is to be searched
 * @return Boolean at the position
 * 
 * @todo Throw Exceptions
 */
bool Bundle::getBooleanAtPos(int pos){
    if(pos > 0 && pos <= currentPos){
        pair<string,BindingType> bindingMeta = this->m_bindingPos[pos];
        if(bindingMeta.second != BindingType::BOOLEAN){
            //...throw an exception
        }
        return getBoolean(bindingMeta.first);
    } else {
        //...throw an out of bounds exception
    }
}

/**
 * @brief Returns string at the given position
 * 
 * @param pos Position at which the string is to be searched
 * @return String at the position
 * 
 * @todo Throw Exceptions
 */
string Bundle::getStringAtPos(int pos){
    if(pos > 0 && pos <= currentPos){
        pair<string,BindingType> bindingMeta = this->m_bindingPos[pos];
        if(bindingMeta.second != BindingType::STRING){
            //...throw an exception
        }
        return getString(bindingMeta.first);
    } else {
        //...throw an out of bounds exception
    }
}


}
