#ifndef _ACL_ROLE_PRIVILEGE_H_
#define _ACL_ROLE_PRIVILEGE_H_

#include <system/database/model/Model.h>

using namespace std;
using namespace tuber;

class AclRolePrivilege: public Model{
public:
	AclRolePrivilege();

	static bool authHasPermision(int entity,int action);
};

#endif