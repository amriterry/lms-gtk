#ifndef _SAVE_AUTHOR_CONTROLLER_H_
#define _SAVE_AUTHOR_CONTROLLER_H_

#include <system/controller/Controller.h>

using namespace std;
using namespace tuber;

class Author;

class SaveAuthorController: public Controller{
public:
	void onCreate(Bundle* package);
	void onStart();
private:
	Author* m_author;

	GtkWidget* m_firstNameEntry;
	GtkWidget* m_middleNameEntry;
	GtkWidget* m_lastNameEntry;
};

#endif