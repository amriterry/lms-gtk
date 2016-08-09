#ifndef _SAVE_USER_CONTROLLER_H_
#define _SAVE_USER_CONTROLLER_H_

#include <system/controller/Controller.h>

using namespace std;
using namespace tuber;

class User;

class SaveUserController: public Controller{
public:
	void onCreate(Bundle* package);
	void onStart();
private:
	bool m_editMode;
	User* m_user;

	string m_selectedUserPath;

	GtkWidget* m_firstNameEntry;
	GtkWidget* m_middleNameEntry;
	GtkWidget* m_lastNameEntry;
	GtkWidget* m_usernameEntry;
	GtkWidget* m_emailEntry;
	GtkWidget* m_passwordEntry;
	GtkWidget* m_passwordConfirmationEntry;
	GtkWidget* m_rolesComboBox;

	GtkListStore* m_rolesListStore;
};

#endif