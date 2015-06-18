#include <string>

#include <gtk/gtk.h>

#include <system/request/Request.h>

#include <system/facades/Facades.h>

using namespace std;

namespace tuber{

/**
 * @brief Request 'ctor'
 */
Request::Request(){
    g_message("Request: created Request object");
    this->m_package = nullptr;
}

/**
 * @brief Setter for the action key
 * 
 * @param actionKey Action key to be set
 */
void Request::setActionKey(string actionKey){
	this->m_actionKey = actionKey;
}

/**
 * @brief Getter for the action key
 * @return ActionKey set for the request
 */
string Request::getActionKey(){
	return this->m_actionKey;
}

/**
 * @brief Setter for request package
 * 
 * @param package Package to carry by the request object
 */
void Request::carryPackage(Bundle* package){
	this->m_package = package;
}

/**
 * @brief Getter for the request package
 * @return Package Returns the package carried by the request
 */
Bundle* Request::getPackage(){
	return this->m_package;
}

/**
 * @brief Request Destructor
 */
Request::~Request(){
    //  Deleting the package carried by the request
    if(this->m_package != nullptr){
        App::destroy(this->m_package);
    }
}

}

