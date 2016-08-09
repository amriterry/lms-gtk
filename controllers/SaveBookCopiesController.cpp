#include "SaveBookCopiesController.h"

#include <models/BookCopy.h>

void SaveBookCopiesController::onCreate(Bundle* package){
	Controller::onCreate(package);
	setTopLevelView("saveBookCopies.glade");

	this->m_copyCodeEntry = getSceneObj("copyCodeEntry");

	if(package != nullptr){
		if(package->hasInt("copy_id")){
			this->m_editMode = true;
	        this->m_copy = BookCopy::find<BookCopy>(package->getInt("copy_id"));
	        gtk_entry_set_text(GTK_ENTRY(m_copyCodeEntry),m_copy->fetch()->getString("copy_code").c_str());
		} else {
			this->m_copy = new BookCopy();

			if(package->hasInt("book_id")){
				this->m_copy->getDirty()
    				->putExtra("book_id",package->getInt("book_id"));
			}
		}
	}
}

void SaveBookCopiesController::onStart(){
	Controller::onStart();

	int rc;

	Bundle* returnInstance = nullptr;

	while((rc = gtk_dialog_run(GTK_DIALOG(getSceneRoot()))) == GTK_RESPONSE_OK){
        int bookId;
	    if(m_editMode){
            bookId = m_copy->fetch()->getInt("book_id");
	    } else {
            bookId = m_copy->getDirty()->getInt("book_id");
	    }

		string code = gtk_entry_get_text(GTK_ENTRY(m_copyCodeEntry));
		vector<string> errors;
		if(code == ""){
			errors.push_back("<b>Book copy code should not be empty</b>");
		}

		if(!errors.empty()){
			GtkWidget* dialog = getSceneObj("errorDialog");
			gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(dialog),Strings::join(errors,"\n").c_str());
			gtk_dialog_run(GTK_DIALOG(dialog));
			gtk_widget_hide(dialog);
			continue;
		}

		this->m_copy->getDirty()
				->putExtra("copy_code",code);

		if(!m_editMode){
			this->m_copy->getDirty()
						->putExtra("status_id",BookCopy::STATUS_ISSUEABLE);
		}

		if(this->m_copy->saveRow()){
			g_message("SaveBookCopiesController: Saved Book id [%i]",bookId);
			returnInstance = BundleFacade::get();
			returnInstance->putExtra("bookCopySaved",true)
							->putExtra("book_id",bookId);
			break;
		}
	}

	finish(returnInstance);
}

void SaveBookCopiesController::onStop(){
	delete m_copy;
}
