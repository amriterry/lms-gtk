#include "BookAuthor.h"

BookAuthor::BookAuthor(){
    setModelAttributes("book_authors","reln_id",false);
}

void BookAuthor::getAuthorsFromBook(int bookId,RowCallback callback,Bundle* data){
    BookAuthor::select({
        "authors.author_id",
        "authors.first_name",
        "authors.middle_name",
        "authors.last_name"
    })
    ->from("book_authors")
    ->join("authors","book_authors.author_id = authors.author_id")
    ->where("book_authors.book_id","=",bookId)
    ->get(callback,data);
}

list<int> BookAuthor::getAuthorListFromBook(int bookId){
    list<int> selected;

    BookAuthor::getAuthorsFromBook(bookId,[&](Statement* stmt,Bundle* data){
        selected.push_back(stmt->getInt("author_id"));
    });

    return selected;
}

void BookAuthor::deleteForBook(int bookId){
    BookAuthor::table("book_authors")
        ->where("book_authors.book_id","=",bookId)
        ->deleteData();
}

bool BookAuthor::isAuthorBinded(int authorId){
    QueryRow row = BookAuthor::select({"*"})
                            ->from("book_authors")
                            ->where("book_authors.author_id","=",authorId)
                            ->first();

    if(row->empty()){
        return false;
    } else {
        return true;
    }
}
