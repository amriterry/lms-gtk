#ifndef _UNISSUE_BOOK_COPY_CONTROLLER_H_
#define _UNISSUE_BOOK_COPY_CONTROLLER_H_

#include <system/controller/Controller.h>

using namespace tuber;

class UnissueBookCopyController: public Controller{
public:
	void onCreate(Bundle* package);
	void onStart();
private:
	QueryRow m_issued;
	bool m_renewMode;

	GtkWidget* m_bookName;
	GtkWidget* m_copyCode;
	GtkWidget* m_userName;
	GtkWidget* m_firstIssuedAt;
	GtkWidget* m_lastIssuedAt;
	GtkWidget* m_deadlineAt;
	GtkWidget* m_fineContainer;
	GtkWidget* m_fine;
};


#endif
