#include "IssueBookController.h"

#include <models/User.h>
#include <models/BookCopy.h>
#include <models/Issue.h>

void IssueBookController::onCreate(Bundle* package){
    Controller::onCreate(package);
	setTopLevelView("issuebook.glade");

	m_bookName = getSceneObj("bookName");
	m_copyCode = getSceneObj("bookCopyCode");
	m_userComboBox = getSceneObj("userComboBox");
	m_userListStore = GTK_LIST_STORE(getSceneRawObj("userListStore"));

	if(package != nullptr && package->hasInt("copyId")){
        this->m_copyId = package->getInt("copyId");
		m_bookCopy = BookCopy::getCopyInfo(m_copyId);

		gtk_label_set_text(GTK_LABEL(m_bookName),m_bookCopy->getString("book_name").c_str());
		gtk_label_set_text(GTK_LABEL(m_copyCode),m_bookCopy->getString("copy_code").c_str());
	}
}

void IssueBookController::onStart(){
    Controller::onStart();
    GtkTreeIter iter;
    User::getIssueableUsers([&](Statement* stmt,Bundle* data){
    	vector<string> names = {stmt->getString("first_name"),stmt->getString("middle_name"),stmt->getString("last_name")};
    	string name = Strings::join(names," ");
    	gtk_list_store_append(m_userListStore,&iter);
    	gtk_list_store_set(m_userListStore,&iter,0,Strings::toString(stmt->getInt("user_id")).c_str(),
    											1,name.c_str(),-1);
    });

    gtk_combo_box_set_active(GTK_COMBO_BOX(m_userComboBox),0);

    int bookId = 0;
	int rc = gtk_dialog_run(GTK_DIALOG(getSceneRoot()));
    if(rc == GTK_RESPONSE_OK){
        g_message("Response OK");

        int userId = atoi(gtk_combo_box_get_active_id(GTK_COMBO_BOX(m_userComboBox)));

        DB::transaction([&](){

            Issue* issue = new Issue();

            issue->getDirty()
                    ->putExtra("copy_id",m_copyId)
                    ->putExtra("user_id",userId)
                    ->putExtra("issued_at",DateTime::getCurrentDateTime())
                    ->putExtra("deadline_at",DateTime::getFutureTime(Issue::DEADLINE_FIRST,DateTimeParams::DAY))
                    ->putExtra("status_id",Issue::STATUS_ISSUED);

            if(!issue->saveRow()){
                throw exception();
            }

            BookCopy* bookCopy = BookCopy::find<BookCopy>(m_copyId);
            bookId = bookCopy->fetch()->getInt("book_id");
            g_message("%i BookId",bookId);
            bookCopy->getDirty()
                ->putExtra("status_id",BookCopy::STATUS_ISSUED);

            if(!bookCopy->saveRow()){
                throw exception();
            }

            delete issue;
            delete bookCopy;
        });
    }
    Bundle* returnCollection = BundleFacade::get();
    returnCollection->putExtra("issuesChanged",true)
                    ->putExtra("book_id",bookId);
    finish(returnCollection);
}
