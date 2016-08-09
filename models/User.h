#ifndef _USER_H_
#define _USER_H_

#include <system/database/model/Model.h>
#include <system/auth/UserInterface.h>

using namespace tuber;

class User: public Model,public UserInterface{
public:
	User();

	string getAuthIdentifierColumnName();
	string getAuthPasswordColumnName();
	string getAuthIdentifier();
	string getAuthPassword();
	int getAuthId();
	void clearAuthAttr();
	void syncAuthAttr(QueryRow row);

	static void getIssueableUsers(RowCallback callback,Bundle* data = nullptr);

	static void getValidUsers(RowCallback callback,Bundle* data = nullptr);
	static void getUsersForId(vector<int> ids,RowCallback callback,Bundle* data = nullptr);

	constexpr static int STATUS_VALID = 1;
	constexpr static int STATUS_EXPIRED = 2;
};

#endif
