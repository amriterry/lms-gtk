#ifndef _ROLE_H_
#define _ROLE_H_

#include <system/database/model/Model.h>

using namespace std;
using namespace tuber;

class Role: public Model{
public:
    Role();

    constexpr static int ROLE_ADMINISTRATOR = 1;
    constexpr static int ROLE_LIBRARIAN = 2;
    constexpr static int ROLE_STUDENT = 3;
};

#endif