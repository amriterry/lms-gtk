#ifndef _USER_INTERFACE_H_
#define _USER_INTERFACE_H_

namespace tuber{

class UserInterface{
public:
	virtual string getAuthIdentifierColumnName() = 0;
	virtual string getAuthPasswordColumnName() = 0;
	virtual string getAuthIdentifier() = 0;
	virtual string getAuthPassword() = 0;
	virtual int getAuthId() = 0;
	virtual void clearAuthAttr() = 0;
	virtual void syncAuthAttr(QueryRow row) = 0;
};

}

#endif
