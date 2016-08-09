#ifndef _BOOK_COPY_H_
#define _BOOK_COPY_H_

#include <system/database/model/Model.h>

using namespace std;
using namespace tuber;

class BookCopy: public Model{
public:
    BookCopy();
    static QueryRow getCopyInfo(int copyId);
    static void getCopiesForBook(int bookId,RowCallback callback,Bundle* data = nullptr);
    static void deleteForBook(int bookId);
    static string getStatusString(int status_id);
    static bool hasIssuedForBook(int bookId);
    static vector<int> getCopyIds(int bookId);

    constexpr static int STATUS_ISSUED = 2;
    constexpr static int STATUS_ISSUEABLE = 1;
    constexpr static char* STATUS_STRING_ISSUED = "Issued";
    constexpr static char* STATUS_STRING_ISSUEABLE = "Available";
};

#endif
