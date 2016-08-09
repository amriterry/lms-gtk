#ifndef _AUTH_MANAGER_H_
#define _AUTH_MANAGER_H_

#include <system/foundation/Service.h>

#include <config/AuthConfig.h>

namespace tuber{

class AuthManager: public Service{
public:
	AuthManager();
	bool check(string username,string password);
	bool isLoggedIn();
	void logout();
	int id();
	UserInterface* getModel();
	~AuthManager();
private:
	bool m_loggedIn;
	UserInterface* m_model;
};

}

#endif