#include "SaveUserController.h"

#include <models/User.h>
#include <models/Role.h>

void SaveUserController::onCreate(Bundle* package){
	Controller::onCreate(package);

	setTopLevelView("saveUser.glade");

	this->m_firstNameEntry = getSceneObj("firstNameEntry");
	this->m_middleNameEntry = getSceneObj("middleNameEntry");
	this->m_lastNameEntry = getSceneObj("lastNameEntry");
	this->m_usernameEntry = getSceneObj("usernameEntry");
	this->m_emailEntry = getSceneObj("emailEntry");
	this->m_passwordEntry = getSceneObj("passwordEntry");
	this->m_passwordConfirmationEntry = getSceneObj("passwordConfirmationEntry");
	this->m_rolesComboBox = getSceneObj("rolesComboBox");

	this->m_rolesListStore = GTK_LIST_STORE(getSceneRawObj("rolesListStore"));

    GtkTreeIter iter;
	Role::all<Role>([&](Statement* stmt,Bundle* pacakge){
        gtk_list_store_append(m_rolesListStore,&iter);
        gtk_list_store_set(m_rolesListStore,&iter,
                           0,Strings::toString(stmt->getInt("role_id")).c_str(),
                           1,stmt->getString("role_name").c_str(),-1);
    });

    gtk_combo_box_set_active(GTK_COMBO_BOX(m_rolesComboBox),0);

	if(package != nullptr && package->hasInt("user_id")){
		m_editMode = true;

		gtk_widget_hide(getSceneObj("passwordContainer"));
		gtk_widget_hide(getSceneObj("passwordConfirmationContainer"));

		m_user = User::find<User>(package->getInt("user_id"));

		gtk_entry_set_text(GTK_ENTRY(m_firstNameEntry),m_user->fetch()->getString("first_name").c_str());
		gtk_entry_set_text(GTK_ENTRY(m_middleNameEntry),m_user->fetch()->getString("middle_name").c_str());
		gtk_entry_set_text(GTK_ENTRY(m_lastNameEntry),m_user->fetch()->getString("last_name").c_str());
		gtk_entry_set_text(GTK_ENTRY(m_usernameEntry),m_user->fetch()->getString("username").c_str());
		gtk_entry_set_text(GTK_ENTRY(m_emailEntry),m_user->fetch()->getString("email").c_str());

		gtk_combo_box_set_active_id(GTK_COMBO_BOX(m_rolesComboBox),Strings::toString(m_user->fetch()->getInt("role_id")).c_str());

		if(package->hasString("selectedUserPath")){
			m_selectedUserPath = package->getString("selectedUserPath");
		}
	} else {
		m_editMode = false;

		m_user = new User();
	}
}

void SaveUserController::onStart(){

	Bundle* packageToReturn = nullptr;

	int rc;
	while((rc = gtk_dialog_run(GTK_DIALOG(getSceneRoot()))) == GTK_RESPONSE_OK){

		string firstName = gtk_entry_get_text(GTK_ENTRY(m_firstNameEntry));
		string middleName = gtk_entry_get_text(GTK_ENTRY(m_middleNameEntry));
		string lastName = gtk_entry_get_text(GTK_ENTRY(m_lastNameEntry));
		string username = gtk_entry_get_text(GTK_ENTRY(m_usernameEntry));
		string email = gtk_entry_get_text(GTK_ENTRY(m_emailEntry));
		string password = gtk_entry_get_text(GTK_ENTRY(m_passwordEntry));
		string passwordConfirmation = gtk_entry_get_text(GTK_ENTRY(m_passwordConfirmationEntry));
		int roleId = atoi(gtk_combo_box_get_active_id(GTK_COMBO_BOX(m_rolesComboBox)));

		vector<string> errors;

		if(firstName == ""){
			errors.push_back("<b>First Name should not be empty</b>");
		}
		if(lastName == ""){
			errors.push_back("<b>Last Name should not be empty</b>");
		}
		if(username == ""){
			errors.push_back("<b>Username should not be empty</b>");
		}
		if(!m_editMode){
			if(password == ""){
				errors.push_back("<b>Password should be empty</b>");
			}
			if(passwordConfirmation == ""){
				errors.push_back("<b>Re-entered password should be empty</b>");
			}
			if(password != passwordConfirmation){
				errors.push_back("<b>Passwords do not match</b>");
			}
		}

		if(!errors.empty()){
			GtkDialog* errorDialog = GTK_DIALOG(getSceneObj("errorDialog"));
			gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(errorDialog),Strings::join(errors,"\n").c_str());
			gtk_dialog_run(errorDialog);
			gtk_widget_hide(GTK_WIDGET(errorDialog));
			continue;
		}

		m_user->getDirty()
			->putExtra("first_name",firstName)
			->putExtra("last_name",lastName)
			->putExtra("username",username)
			->putExtra("role_id",roleId);

		if(middleName != ""){
			m_user->getDirty()
					->putExtra("middle_name",middleName);
		}

		if(email != ""){
			m_user->getDirty()
				->putExtra("email",email);
		}

		if(!m_editMode){
			m_user->getDirty()
				->putExtra("password",Hash::make(password))
				->putExtra("status_id",User::STATUS_VALID);
		}

		if(m_user->saveRow()){
			packageToReturn = BundleFacade::get();
			packageToReturn->putExtra("user_id",m_user->fetch()->getInt("user_id"))
							->putExtra("userSaved",true);

			if(m_selectedUserPath != ""){
				packageToReturn->putExtra("selectedUserPath",m_selectedUserPath);
			}
			break;
		}
	}

	finish(packageToReturn);
}
