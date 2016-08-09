#ifndef _AUTH_H_
#define _AUTH_H_

#include <system/facades/Facade.h>

#include <system/auth/AuthManager.h>

namespace tuber{

class Auth: public Facade{
public:
	static AuthManager* get(){
		return m_app->resolve<AuthManager>(getFacadeAccessor());
	}

	static string getFacadeAccessor(){
        return "AuthManager";
	}

	static bool check(string username,string password){
		return get()->check(username,password);
	}

	static int id(){
		return get()->id();
	}

	static bool isLoggedIn(){
		return get()->isLoggedIn();
	}

	static UserInterface* getModel(){
		return get()->getModel();
	}

	static void logout(){
		get()->logout();
	}
};

}



#endif
