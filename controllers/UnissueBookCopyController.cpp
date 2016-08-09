#include "UnissueBookCopyController.h"

#include <models/Issue.h>
#include <models/BookCopy.h>

void UnissueBookCopyController::onCreate(Bundle* package){
	Controller::onCreate(package);
	setTopLevelView("unissueBookCopy.glade");

	this->m_bookName = getSceneObj("bookNameLabel");
	this->m_copyCode = getSceneObj("copyCodeLabel");
	this->m_userName = getSceneObj("userNameLabel");
	this->m_firstIssuedAt = getSceneObj("firstIssuedAtLabel");
	this->m_lastIssuedAt = getSceneObj("lastIssuedAtLabel");
	this->m_deadlineAt = getSceneObj("deadlineAtLabel");
	this->m_fineContainer = getSceneObj("fineContainer");
	this->m_fine = getSceneObj("fineLabel");

	if(package != nullptr && package->hasInt("copyId")){
        this->m_renewMode = package->getBoolean("renewMode");
		int copyId = package->getInt("copyId");
		vector<string> names;

		m_issued = Issue::getIssueInfoOfCopy(copyId);

		double fine = Issue::calculateFine(m_issued->getString("deadline_at"));

		names.push_back(m_issued->getString("first_name"));
        names.push_back(m_issued->getString("middle_name"));
        names.push_back(m_issued->getString("last_name"));

		gtk_label_set_text(GTK_LABEL(m_bookName),m_issued->getString("book_name").c_str());
		gtk_label_set_text(GTK_LABEL(m_copyCode),m_issued->getString("copy_code").c_str());
		gtk_label_set_text(GTK_LABEL(m_userName),Strings::join(names," ").c_str());
		//gtk_label_set_text(GTK_LABEL(m_firstIssuedAt),)
		gtk_label_set_text(GTK_LABEL(m_lastIssuedAt),m_issued->getString("issued_at").c_str());
		gtk_label_set_text(GTK_LABEL(m_deadlineAt),m_issued->getString("deadline_at").c_str());
		gtk_label_set_text(GTK_LABEL(m_fine),(string("Rs. ") + Strings::toString(fine)).c_str());

		if(!m_renewMode){
            gtk_button_set_label(GTK_BUTTON(getSceneObj("applyBtn")),"Return Book");
		}
	}
}

void UnissueBookCopyController::onStart(){

	int rc = gtk_dialog_run(GTK_DIALOG(getSceneRoot()));
	int bookId = 0;
	if(rc == GTK_RESPONSE_OK){

		//...unissue the book
		DB::transaction([&](){
            Issue* issue;
            if(m_renewMode){
                issue = new Issue();
                issue->getDirty()
                    ->putExtra("copy_id",m_issued->getInt("copy_id"))
                    ->putExtra("user_id",m_issued->getInt("user_id"))
                    ->putExtra("last_issue_id",m_issued->getInt("issue_id"))
                    ->putExtra("issued_at",DateTime::getCurrentDateTime())
                    ->putExtra("deadline_at",DateTime::getFutureTime(Issue::DEADLINE_FIRST,DateTimeParams::DAY))
                    ->putExtra("status_id",Issue::STATUS_RENEWED);

                if(!issue->saveRow()){
                	throw exception();
                }
            } else {
                issue = Issue::find<Issue>(m_issued->getInt("issue_id"));

                issue->getDirty()
                	->putExtra("status_id",Issue::STATUS_RETURNED);


                if(!issue->saveRow()){
                	throw exception();
                }

                BookCopy* copy = BookCopy::find<BookCopy>(m_issued->getInt("copy_id"));
                bookId = copy->fetch()->getInt("book_id");

                copy->getDirty()
                	->putExtra("status_id",BookCopy::STATUS_ISSUEABLE);

                if(!copy->saveRow()){
                    throw exception();
                }
            }
        });
	}
	Bundle* returnPackage = BundleFacade::get();
	returnPackage->putExtra("issuesChanged",true)
				->putExtra("book_id",bookId);
	finish(returnPackage);
}
