#ifndef _SAVE_BOOK_CONTROLLER_H_
#define _SAVE_BOOK_CONTROLLER_H_

#include <system/controller/Controller.h>

using namespace std;
using namespace tuber;

class Book;

class SaveBookController: public Controller{
public:
	void onCreate(Bundle* package);
	void onStart();
	void onPause();
	void onStop();

	void moveSelectedAuthors(GtkTreeView* fromView,GtkListStore* fromStore,GtkTreeView* toView,GtkListStore* toStore);
	void savePublication(bool editMode = false);
	void saveAuthor(bool editMode = false);
	void loadCopyCodes(int bookId);

	void loadAuthors();
	void loadPublications();

	vector<int> getSelectedAuthorsList();

	int getSelectedAuthorId();

	//void moveSelection(GtkTreeView* from,GtkListStore* to);
private:
	bool m_editMode = false;
	string m_selectedBookPath;

	Book* m_book;

	GtkWidget* m_bookName;
	GtkWidget* m_copyCount;
	GtkWidget* m_isbn;
	GtkWidget* m_edition;

	GtkWidget* m_copyCodeContainer;

	GtkWidget* m_addPublicationBtn;
	GtkWidget* m_editPublicationBtn;
	GtkWidget* m_deletePublicationBtn;

	GtkWidget* m_addAuthorBtn;
	GtkWidget* m_editAuthorBtn;
	GtkWidget* m_deleteAuthorBtn;

	GtkComboBox* m_publicationComboBox;

	GtkWidget* m_authorsTreeView;
	GtkWidget* m_selectedAuthorsTreeView;

	GtkWidget* m_addAuthorSelectionBtn;
	GtkWidget* m_removeAuthorSelectionBtn;

	GtkListStore* m_authorsListStore;
	GtkListStore* m_selectedAuthorsListStore;
	GtkListStore* m_publicationListStore;
};

#endif
