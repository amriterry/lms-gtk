#ifndef _ISSUE_BOOK_CONTROLLER_H_
#define _ISSUE_BOOK_CONTROLLER_H_

#include <system/controller/Controller.h>

using namespace std;
using namespace tuber;

class IssueBookController: public Controller{
public:
	void onCreate(Bundle* package);
	void onStart();
private:
	int m_copyId;
	QueryRow m_bookCopy;

	GtkWidget* m_bookName;
	GtkWidget* m_copyCode;
	GtkWidget* m_userComboBox;
	GtkListStore* m_userListStore;
};

#endif