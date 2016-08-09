#include "BookCopy.h"

BookCopy::BookCopy(){
    setModelAttributes("book_copies","copy_id",false);
}

QueryRow BookCopy::getCopyInfo(int copyId){
	return BookCopy::select({
				"book_copies.copy_id",
				"book_copies.copy_code,"
				"book_copies.status_id",
				"books.book_name"
			})
			->from("book_copies")
			->join("books","books.book_id = book_copies.book_id")
			->where("book_copies.copy_id","=",copyId)
			->first();
}

void BookCopy::getCopiesForBook(int bookId,RowCallback callback,Bundle* data){
	BookCopy::select({
		"book_copies.copy_id",
		"book_copies.copy_code",
		"book_copies.status_id"
	})
	->from("book_copies")
	->where("book_copies.book_id","=",bookId)
	->orderBy("book_copies.status_id","desc")
	->get(callback,data);
}

void BookCopy::deleteForBook(int bookId){
	BookCopy::table("book_copies")
			->where("book_copies.book_id","=",bookId)
			->deleteData();
}

string BookCopy::getStatusString(int status_id){
	switch(status_id){
		case STATUS_ISSUED:
			return STATUS_STRING_ISSUED;
			break;
		case STATUS_ISSUEABLE:
			return STATUS_STRING_ISSUEABLE;
			break;
		default:
			return "";
			break;
	}
}

bool BookCopy::hasIssuedForBook(int bookId){
	QueryRow row = BookCopy::select({"*"})
						->from("book_copies")
						->where("book_copies.status_id","=",STATUS_ISSUED)
						->where("book_copies.book_id","=",bookId)
						->first();

	if(row->empty()){
		return false;
	} else {
		return true;
	}
}

vector<int> BookCopy::getCopyIds(int bookId){
	vector<int> ids;

	BookCopy::select({"copy_id"})
			->from("book_copies")
			->where("book_copies.book_id","=",bookId)
			->get([&](Statement* stmt,Bundle* data){
				ids.push_back(stmt->getInt("copy_id"));
			},nullptr);

	return ids;
}
