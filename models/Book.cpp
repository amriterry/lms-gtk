#include "Book.h"
#include <system/database/query/QueryBuilder.h>

Book::Book(){
    setModelAttributes("books","book_id",true);
}

void Book::getBookList(RowCallback callback,Bundle* data){

    Book::select({
                 "books.book_id",
                 "books.book_name",
                 "books.isbn",
                 "books.edition",
                 "books.publication_id",
                 "books.created_at",
                 "books.updated_at",
                 "publications.publication_name",
                 })
            ->from("books")
            ->join("publications","books.publication_id = publications.publication_id")
            ->get(callback,data);
}

void Book::getBookForIds(vector<int> ids,RowCallback callback,Bundle* data){
    QueryBuilder* builder = Book::select({
         "books.book_id",
         "books.book_name",
         "books.isbn",
         "books.edition",
         "books.publication_id",
         "books.created_at",
         "books.updated_at",
         "publications.publication_name",
         })
    ->from("books")
    ->join("publications","books.publication_id = publications.publication_id");

    for(auto id: ids){
        builder->where("books.book_id","=",id,"or");
    }

    builder->get(callback,data);
}

bool Book::hasForPublication(int publicationId){
    QueryRow row = Book::select({"*"})
                        ->from("books")
                        ->where("books.publication_id","=",publicationId)
                        ->first();

    if(row->empty()){
        return false;
    } else {
        return true;
    }
}