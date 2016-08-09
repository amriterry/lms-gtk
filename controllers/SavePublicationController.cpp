#include "SavePublicationController.h"

#include <models/Publication.h>

void SavePublicationController::onCreate(Bundle* package){
	Controller::onCreate(package);
	setTopLevelView("savePublication.glade");

	if(package != nullptr && package->hasInt("publication_id")){
		this->m_publication = Publication::find<Publication>(package->getInt("publication_id"));

		gtk_entry_set_text(GTK_ENTRY(getSceneObj("publicationEntry")),m_publication->fetch()->getString("publication_name").c_str());
	} else {
		this->m_publication = new Publication();
	}
}

void SavePublicationController::onStart(){
	Controller::onStart();

	int rc = gtk_dialog_run(GTK_DIALOG(getSceneRoot()));

	g_message("Checking response %i",rc);
	if(rc == GTK_RESPONSE_OK){
		string publicationName = gtk_entry_get_text(GTK_ENTRY(getSceneObj("publicationEntry")));
		m_publication->getDirty()
					->putExtra("publication_name",publicationName);

		m_publication->saveRow();
	}
	finish();
}

void SavePublicationController::onStop(){
	Controller::onStop();

	delete m_publication;
}
