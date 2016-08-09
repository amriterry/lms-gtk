#include "Issue.h"

#include <gtk/gtk.h>

Issue::Issue(){
	this->setModelAttributes("issues","issue_id",false);
}

QueryRow Issue::getIssueInfoOfCopy(int copyId){
	return Issue::select({
		"issues.issue_id",
		"issues.copy_id",
		"issues.user_id",
		"issues.issued_at",
		"issues.deadline_at",
		"users.first_name",
		"users.middle_name",
		"users.last_name",
		"books.book_name",
		"book_copies.copy_code"
	})
	->from("issues")
	->join("users","users.user_id = issues.user_id")
	->join("book_copies","book_copies.copy_id = issues.copy_id")
	->join("books","books.book_id = book_copies.book_id")
	->where("issues.copy_id","=",copyId)
	->orderBy("issues.issued_at","desc")
	->first();
}

void Issue::getIssues(RowCallback callback,Bundle* data){
	Issue::select({
		"issues.issue_id",
		"issues.copy_id",
		"issues.user_id",
		"issues.issued_at",
		"issues.deadline_at",
		//"issues.returned_at",
		"users.first_name",
		"users.middle_name",
		"users.last_name",
		"books.book_name",
		"book_copies.copy_code",
		"book_copies.status_id"
	})
	->from("issues")
	->join("users","users.user_id = issues.user_id")
	->join("book_copies","book_copies.copy_id = issues.copy_id")
	->join("books","books.book_id = book_copies.book_id")
	->groupBy(vector<string>({"issues.copy_id","issues.user_id"}))
	->orderBy("issued_at","desc")
	->get(callback,data);
}

void Issue::deleteForCopies(vector<int> copyIds){

	QueryBuilder* builder = Issue::table("issues");

	for(auto copyId: copyIds){
		builder->where("copy_id","=",copyId,"or");
	}
		
	builder->deleteData();
}

void Issue::deleteForUser(int userId){
	Issue::table("issues")
			->where("issues.user_id","=",userId)
			->deleteData();
}

bool Issue::hasIssuesForUser(int userId){
	QueryRow row = Issue::select({"*"})
						->from("issues")
						->where("issues.user_id","=",userId)
						->where("issues.status_id","=",STATUS_ISSUED)
						->first();
	return !row->empty();
}

double Issue::calculateFine(string deadlineDate){
	double fine = 0.0;
	int numDays;
	int diff = DateTime::getTimeDiff(DateTimeParams::DAY,deadlineDate);
	if(diff < 0.0){
		numDays =  - ((int) diff);
	} else {
		numDays = 0;
	}

	if(numDays < (DEADLINE_SECOND - DEADLINE_FIRST)){
		fine = numDays * FINE_RATE_FIRST;
	} else if(numDays < (DEADLINE_THIRD - DEADLINE_FIRST)){
		fine = (DEADLINE_SECOND - DEADLINE_FIRST) * FINE_RATE_FIRST;
		fine += (numDays - (DEADLINE_SECOND - DEADLINE_FIRST)) * FINE_RATE_SECOND;
	} else {
		fine = (DEADLINE_SECOND - DEADLINE_FIRST) * FINE_RATE_FIRST;
		fine += (numDays - (DEADLINE_SECOND - DEADLINE_FIRST)) * FINE_RATE_SECOND;
		fine += (numDays - (DEADLINE_THIRD - DEADLINE_FIRST)) * FINE_RATE_THIRD;
	}

	return fine;
}
