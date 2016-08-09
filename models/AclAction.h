#ifndef _ACL_ACTION_H_
#define _ACL_ACTION_H_

#include <system/database/model/Model.h>

using namespace std;
using namespace tuber;

class AclAction: public Model{
public:
	AclAction();

	constexpr static int ACTION_CREATE = 1;
	constexpr static int ACTION_READ = 2;
	constexpr static int ACTION_UPDATE = 3;
	constexpr static int ACTION_DELETE = 4;
};

#endif
