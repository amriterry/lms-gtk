#ifndef _BOOK_H_
#define _BOOK_H_

#include <system/database/model/Model.h>

using namespace std;
using namespace tuber;

class Book: public Model{
public:
    Book();
    static void getBookList(RowCallback callback,Bundle* data = nullptr);
    static void getBookForIds(vector<int> ids,RowCallback callback,Bundle* data = nullptr);
    static bool hasForPublication(int publicationId);
};

#endif