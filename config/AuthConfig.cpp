#include "AuthConfig.h"

#include <models/User.h>
#include <system/auth/UserInterface.h>

UserInterface* AuthConfig::model = nullptr;

string AuthConfig::table = "users";

UserInterface* AuthConfig::getInterface(){
    if(model == nullptr){
        model = new User();
    }
	return model;
}

string AuthConfig::getTable(){
	return table;
}
