#ifndef _AUTHOR_H_
#define _AUTHOR_H_

#include <system/database/model/Model.h>

using namespace std;
using namespace tuber;

class Author: public Model{
public:
   	Author();
    static void getAuthorList(RowCallback callback,Bundle* data = nullptr);
};


#endif