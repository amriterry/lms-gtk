#include "SaveAuthorController.h"

#include <models/Author.h>

void SaveAuthorController::onCreate(Bundle* package){
	Controller::onCreate(package);
	setTopLevelView("saveAuthorView.glade");

	this->m_firstNameEntry = this->getSceneObj("firstNameEntry");
	this->m_middleNameEntry = this->getSceneObj("middleNameEntry");
	this->m_lastNameEntry = this->getSceneObj("lastNameEntry");

	if(package != nullptr && package->hasInt("author_id")){
		this->m_author = Author::find<Author>(package->getInt("author_id"));

		gtk_entry_set_text(GTK_ENTRY(m_firstNameEntry),m_author->fetch()->getSafeString("first_name").c_str());
		gtk_entry_set_text(GTK_ENTRY(m_middleNameEntry),m_author->fetch()->getSafeString("middle_name").c_str());
		gtk_entry_set_text(GTK_ENTRY(m_lastNameEntry),m_author->fetch()->getSafeString("last_name").c_str());
	} else {
		this->m_author = new Author();
	}
}

void SaveAuthorController::onStart(){
	Controller::onStart();

    int rc;
	while((rc = gtk_dialog_run(GTK_DIALOG(getSceneRoot()))) == GTK_RESPONSE_OK){
		string firstName = gtk_entry_get_text(GTK_ENTRY(m_firstNameEntry));
		string middleName = gtk_entry_get_text(GTK_ENTRY(m_middleNameEntry));
		string lastName = gtk_entry_get_text(GTK_ENTRY(m_lastNameEntry));

		if(firstName == "" || lastName == ""){
            GtkWidget* dialog = getSceneObj("errorDialog");
            gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(dialog),"<b>First name and last name cannot be empty.</b>");
			gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_hide(dialog);
		} else {
			m_author->getDirty()
					->putExtra("first_name",firstName)
					->putExtra("middle_name",middleName)
					->putExtra("last_name",lastName);

			m_author->saveRow();
			break;
		}
	}
	finish();
}
