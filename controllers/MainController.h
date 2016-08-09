#ifndef _MAIN_CONTROLLER_H_
#define _MAIN_CONTROLLER_H_

#include <system/controller/Controller.h>

using namespace tuber;

class MainController : public Controller{
public:
    void onCreate(Bundle* package);
    void onStart();
    void onPause();
    void onResume(Bundle* package);

    void loadBooks();
    void loadBookInfo(GtkTreeModel* treeModel,GtkTreeIter* iter);
    void loadAuthors(int book_id);
    void loadBookCopies(int book_id);

    void loadIssues();
    void loadUsers();

    void loadTrees();

    void appendBookFromStmt(Statement* stmt);
    void setBookFromStmt(Statement* stmt,GtkTreeIter* iter);

    void appendUserFromStmt(Statement* stmt);
    void setUserFromStmt(Statement* stmt,GtkTreeIter* iter);

    void unsetBookCopyOptionsSensitivity();
    void normalizeBookCopyOptions(GtkTreeSelection* selection);

    void unsetUserOptionsSensitivity();
    void normalizeUserOptions(GtkTreeSelection* selection);

    void normalizePermisions();

    int getSelectedBookIdFromTree();
    int getSelectedBookCopyIdFromTree();
    int getSelectedUserIdFromTree();

    string getSelectedBookPath();
    string getSelectedBookCopyPath();
    string getSelectedUserPath();

    void saveBook(bool editMode);
    void saveBookCopy(bool editMode);
    void saveUser(bool editMode);
private:
    bool m_canFinish;
    bool m_isFirstLoaded;
    GtkWidget* m_userNameLabel;
    GtkWidget* m_logOutBtn;

    GtkWidget* m_mainNotebook;

    GtkWidget* m_searchByComboBox;
    GtkWidget* m_searchEntry;
    GtkWidget* m_searchUserEntry;
    GtkWidget* m_searchIssueEntry;
    GtkWidget* m_searchIssueByComboBox;

    GtkWidget* m_addNewBookBtn;
    GtkWidget* m_editBookBtn;
    GtkWidget* m_deleteBookBtn;
    GtkWidget* m_addUserBtn;
    GtkWidget* m_editUserBtn;
    GtkWidget* m_deleteUserBtn;

    GtkWidget* m_addNewBookCopyBtn;
    GtkWidget* m_editBookCopyBtn;
    GtkWidget* m_deleteBookCopyBtn;
    GtkWidget* m_issueBookCopyBtn;
    GtkWidget* m_renewBookCopyBtn;
    GtkWidget* m_returnBookCopyBtn;

    GtkWidget* m_bookInfoFrame;

    GtkWidget* m_booksTreeView;
    GtkWidget* m_bookCopiesTreeView;
    GtkWidget* m_usersTreeView;
    GtkWidget* m_issuesTreeView;

    GtkWidget* m_bookIdLabel;
    GtkWidget* m_bookNameLabel;
    GtkWidget* m_isbnLabel;
    GtkWidget* m_editionLabel;
    GtkWidget* m_publicationLabel;
    GtkWidget* m_authorsLabel;
    GtkWidget* m_addedAtLabel;

    GtkListStore* m_booksListStore;
    GtkListStore* m_bookCopiesListStore;
    GtkListStore* m_usersListStore;
    GtkListStore* m_issuesListStore;

    GtkTreeIter m_userIterator;
    GtkTreeIter m_bookIterator;
    GtkTreeIter m_bookCopyItetrator;
};

#endif //_MAIN_CONTROLLER_H_
