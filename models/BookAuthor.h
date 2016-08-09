#ifndef _BOOK_AUTHOR_H_
#define _BOOK_AUTHOR_H_

#include <system/database/model/Model.h>

#include <list>

using namespace std;
using namespace tuber;

class BookAuthor: public Model{
public:
    BookAuthor();
    static void getAuthorsFromBook(int bookId,RowCallback callback,Bundle* data = nullptr);
    static list<int> getAuthorListFromBook(int bookId);
    static void deleteForBook(int bookId);
    static bool isAuthorBinded(int authorId);
};

#endif