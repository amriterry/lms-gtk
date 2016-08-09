#include "AclRolePrivilege.h"

#include <system/facades/Facades.h>
#include <system/auth/UserInterface.h>
#include <models/User.h>

AclRolePrivilege::AclRolePrivilege(){
    setModelAttributes("aclrole_privileges","entity_id",false);
}

bool AclRolePrivilege::authHasPermision(int entity,int action){
	User* user = dynamic_cast<User*>(Auth::getModel());
	int roleId = user->fetch()->getInt("role_id");

	QueryRow row = AclRolePrivilege::select({"*"})
							->from("aclrole_privileges")
							->join("roles","roles.role_id = aclrole_privileges.role_id")
							->join("aclprivileges","aclprivileges.privilege_id = aclrole_privileges.privilege_id")
							->join("aclentities","aclentities.entity_id = aclprivileges.entity_id")
							->join("aclactions","aclactions.action_id = aclprivileges.action_id")
							->where("aclentities.entity_id","=",entity)
							->where("aclactions.action_id","=",action)
							->where("aclrole_privileges.role_id","=",roleId)
							->first();

	return !row->empty();
}
