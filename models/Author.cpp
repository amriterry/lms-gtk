#include "Author.h"

Author::Author(){
    setModelAttributes("authors","author_id",true);
}

void Author::getAuthorList(RowCallback callback,Bundle* data){
    Author::select({
                 "authors.author_id",
                 "authors.first_name",
                 "authors.middle_name",
                 "authors.last_name"
                 })
            ->from("authors")
            ->get(callback,data);
}
