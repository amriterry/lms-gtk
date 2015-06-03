#include <string>

#include <gtk/gtk.h>

#include <system/request/Request.h>

using namespace std;

namespace tuber{

void Request::setActionKey(string actionKey){
	this->m_actionKey = actionKey;
}

string Request::getActionKey(){
	return this->m_actionKey;
}

void Request::carryPackage(Bundle* package){
	this->m_package = package;
}

Bundle* Request::getPackage(){
	return this->m_package;
}

Request::~Request(){
    g_message("Request: Deleted request object of action key: %s",m_actionKey.c_str());
}

}

