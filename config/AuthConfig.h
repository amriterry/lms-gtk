#ifndef _AUTH_CONFIG_H_
#define _AUTH_CONFIG_H_

#include <string>

using namespace std;

namespace tuber{

class UserInterface;

class AuthConfig{
public:
	static UserInterface* getInterface();
	static string getTable();
private:
	static UserInterface* model;
	static string table;
};

}

#endif
