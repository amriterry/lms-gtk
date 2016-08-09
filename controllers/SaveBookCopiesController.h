#ifndef _SAVE_BOOK_COPIES_CONTROLLER_H_
#define _SAVE_BOOK_COPIES_CONTROLLER_H_

#include <system/controller/Controller.h>

using namespace std;
using namespace tuber;

class BookCopy;

class SaveBookCopiesController: public Controller{
public:
	void onCreate(Bundle* package);
	void onStart();
	void onStop();
private:
	bool m_editMode = false;
	BookCopy* m_copy;

	GtkWidget* m_copyCodeEntry;
};

#endif