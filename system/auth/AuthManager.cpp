#include "AuthManager.h"

#include <system/database/connections/QueryResult.h>

#include <system/facades/Facades.h>

#include <system/auth/UserInterface.h>

namespace tuber{

AuthManager::AuthManager(){
	this->m_model = nullptr;
	this->m_loggedIn = false;
}

bool AuthManager::check(string username,string password){
	if(m_model == nullptr){
		m_model = AuthConfig::getInterface();
	}
	if(!this->m_loggedIn){
		QueryRow row = DB::table(AuthConfig::getTable())
						->select({"*"})
						->where(m_model->getAuthIdentifierColumnName(),"=",username)
						->where(m_model->getAuthPasswordColumnName(),"=",Hash::make(password))
						->first();
		if(!row->empty()){
			m_model->syncAuthAttr(row);
			this->m_loggedIn = true;
		}
	}
	return this->m_loggedIn;
}

bool AuthManager::isLoggedIn(){
	return this->m_loggedIn;
}

void AuthManager::logout(){
	if(this->m_loggedIn){
		this->m_model->clearAuthAttr();
		this->m_loggedIn = false;
	}
}

int AuthManager::id(){
	if(this->m_loggedIn){
		return this->m_model->getAuthId();
	}
	return 0;
}

UserInterface* AuthManager::getModel(){
	return this->m_model;
}

AuthManager::~AuthManager(){
	delete this->m_model;
}

}
