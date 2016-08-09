#ifndef _ACL_ENTITY_H_
#define _ACL_ENTITY_H_

#include <system/database/model/Model.h>

using namespace std;
using namespace tuber;

class AclEntity: public Model{
public:
	AclEntity();

	constexpr static int ENTITY_BOOK = 1;
	constexpr static int ENTITY_ISSUE = 2;
	constexpr static int ENTITY_USER = 3;
	constexpr static int ENTITY_AUTHOR = 4;
	constexpr static int ENTITY_PUBLICATION = 5;
	constexpr static int ENTITY_BOOK_COPY = 6;
};

#endif