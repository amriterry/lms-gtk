#include <gtk/gtk.h>

#include "MainController.h"

#include <system/facades/Facades.h>
#include <models/User.h>
#include <models/Book.h>
#include <models/Issue.h>
#include <models/BookCopy.h>
#include <models/BookAuthor.h>
#include <models/AclEntity.h>
#include <models/AclAction.h>
#include <models/AclRolePrivilege.h>

using namespace std;
using namespace tuber;

void MainController::onCreate(Bundle* package){
    Controller::onCreate(package);

    setContentView("main.glade");

    //Initializing variables
    this->m_canFinish = false;
    this->m_isFirstLoaded = false;

    this->m_userNameLabel = getSceneObj("userNameLabel");
    this->m_logOutBtn = getSceneObj("logOutBtn");

    this->m_mainNotebook = getSceneObj("mainNoteBook");

    this->m_searchByComboBox = getSceneObj("searchByComboBox");
    this->m_searchEntry = getSceneObj("searchEntry");

    this->m_searchUserEntry = getSceneObj("searchUserEntry");

    this->m_searchIssueEntry = getSceneObj("searchIssueEntry");
    this->m_searchIssueByComboBox = getSceneObj("searchIssueByComboBox");

    this->m_addNewBookBtn = getSceneObj("addNewBookBtn");
    this->m_editBookBtn = getSceneObj("editBookBtn");
    this->m_deleteBookBtn = getSceneObj("deleteBookBtn");

    this->m_addUserBtn = getSceneObj("addUserBtn");
    this->m_editUserBtn = getSceneObj("editUserBtn");
    this->m_deleteUserBtn = getSceneObj("deleteUserBtn");

    this->m_addNewBookCopyBtn = getSceneObj("addNewBookCopyBtn");
    this->m_editBookCopyBtn = getSceneObj("editBookCopyBtn");
    this->m_deleteBookCopyBtn = getSceneObj("deleteBookCopyBtn");
    this->m_issueBookCopyBtn = getSceneObj("issueBookCopyBtn");
    this->m_renewBookCopyBtn = getSceneObj("renewBookCopyBtn");
    this->m_returnBookCopyBtn = getSceneObj("returnBookCopyBtn");

    this->m_bookIdLabel = getSceneObj("bookIdLabel");
    this->m_bookNameLabel = getSceneObj("bookNameLabel");
    this->m_isbnLabel = getSceneObj("isbnLabel");
    this->m_editionLabel = getSceneObj("editionLabel");
    this->m_publicationLabel = getSceneObj("publicationLabel");
    this->m_authorsLabel = getSceneObj("authorsLabel");
    this->m_addedAtLabel = getSceneObj("addedAtLabel");

    this->m_bookInfoFrame = getSceneObj("bookInfoFrame");
    this->m_booksTreeView = getSceneObj("bookTreeView");
    this->m_bookCopiesTreeView = getSceneObj("bookCopiesTreeView");
    this->m_booksListStore = GTK_LIST_STORE(getSceneRawObj("bookStore"));
    this->m_bookCopiesListStore = GTK_LIST_STORE(getSceneRawObj("bookCopiesStore"));

    this->m_usersTreeView = getSceneObj("usersTreeView");
    this->m_usersListStore = GTK_LIST_STORE(getSceneRawObj("usersListStore"));

    this->m_issuesTreeView = getSceneObj("issuesTreeView");
    this->m_issuesListStore = GTK_LIST_STORE(getSceneRawObj("issuesListStore"));

    g_message("MainController: Completed setting up variables");

    //
    //  When the tab of notebook is changed
    //
    void (*pageChangedCallback)(GtkNotebook*,GtkWidget*,guint,gpointer) = [&](GtkNotebook* notebook,GtkWidget* page,guint index,gpointer user_data){
        g_message("Page %i",index);
        MainController* controller = static_cast<MainController*>(user_data);
        controller->loadTrees();
    };
    g_signal_connect(GTK_NOTEBOOK(m_mainNotebook),"switch-page",G_CALLBACK(pageChangedCallback),this);

    //
    //  Hiding Book info section
    //
    gtk_widget_set_visible(this->m_bookInfoFrame,false);

    //
    //  Setting up search entries for each tree view
    //
    gtk_tree_view_set_search_entry(GTK_TREE_VIEW(this->m_booksTreeView),GTK_ENTRY(m_searchEntry));
    gtk_tree_view_set_search_entry(GTK_TREE_VIEW(this->m_issuesTreeView),GTK_ENTRY(m_searchIssueEntry));
    gtk_tree_view_set_search_entry(GTK_TREE_VIEW(this->m_usersTreeView),GTK_ENTRY(m_searchUserEntry));

    gtk_combo_box_set_active(GTK_COMBO_BOX(m_searchIssueByComboBox),0);

    //
    //  When Logout Button is clicked
    //
    GAppEventCallback logOutCallback = [&](GtkWidget* widget,gpointer data){
        MainController* controller = static_cast<MainController*>(data);
        Auth::logout();
        controller->m_canFinish = true;
        controller->startController("LoginController");
    };
    g_signal_connect(m_logOutBtn,"clicked",G_CALLBACK(logOutCallback),this);
    g_message("MainController: Handled Logout Button Callback");

    //
    //  When Book selection is changed
    //
    GtkTreeSelection* bookSelection = gtk_tree_view_get_selection(GTK_TREE_VIEW(m_booksTreeView));
    void (*bookSelectionChanged)(GtkTreeSelection*,gpointer) = [&](GtkTreeSelection *treeselection,gpointer user_data){
        g_message("MainController: Book selection changed");
        GtkTreeIter iter;
        MainController* controller = static_cast<MainController*>(user_data);
        GtkWidget* container = controller->getSceneObj("bookInfoFrame");

        gtk_widget_set_visible(container,false);

        GtkWidget* bookTreeView = controller->getSceneObj("bookTreeView");
        GtkTreeModel* booksModel = gtk_tree_view_get_model(GTK_TREE_VIEW(bookTreeView));
        //GtkTreeSelection* selectedBook = gtk_tree_view_get_selection(GTK_TREE_VIEW(bookTreeView));
        if(gtk_tree_selection_get_selected(treeselection,&booksModel,&iter)){
            controller->loadBookInfo(booksModel,&iter);
            gtk_widget_set_visible(container,true);
        }
    };
    g_signal_connect(bookSelection,"changed",G_CALLBACK(bookSelectionChanged),this);
    g_message("MainController: Handled Book selection changed callback");

    //
    //  When Book copies selection is changed
    //
    GtkTreeSelection* copySelection = gtk_tree_view_get_selection(GTK_TREE_VIEW(m_bookCopiesTreeView));
    void (*copySelectionChanged)(GtkTreeSelection*,gpointer) = [&](GtkTreeSelection* selection,gpointer user_data){
        g_message("MainController: Book copy selection changed");
        MainController* controller = static_cast<MainController*>(user_data);
        controller->normalizeBookCopyOptions(selection);
    };
    g_signal_connect(copySelection,"changed",G_CALLBACK(copySelectionChanged),this);
    g_message("MainController: Handled Copies Selection changed callback");

    //
    //  When User selection is changed
    //
    GtkTreeSelection* userSelection = gtk_tree_view_get_selection(GTK_TREE_VIEW(m_usersTreeView));
    void (*userSelectionChanged)(GtkTreeSelection*,gpointer) = [&](GtkTreeSelection* selection,gpointer data){
        MainController* controller = static_cast<MainController*>(data);
        controller->normalizeUserOptions(selection);
    };
    g_signal_connect(userSelection,"changed",G_CALLBACK(userSelectionChanged),this);

    //
    //  When Search By For Books combo box selection is changed
    //
    GAppEventCallback searchByChangedCallback = [&](GtkWidget* widget,gpointer data){
        g_message("MainController: search by option changed");

        int columnPos = atoi(gtk_combo_box_get_active_id(GTK_COMBO_BOX(widget)));
        gtk_tree_view_set_search_column(GTK_TREE_VIEW(data),columnPos);
    };
    g_signal_connect(m_searchByComboBox,"changed",G_CALLBACK(searchByChangedCallback),this->m_booksTreeView);
    g_message("MainController: Handled Search By parameter changed callback");

    //
    //  When Search By For Issues combo box selection is changed
    //
    GAppEventCallback searchIssueByChangedCallback = [&](GtkWidget* widget,gpointer data){
        int columnPos = gtk_combo_box_get_active(GTK_COMBO_BOX(widget));
        int searchPos = -1;
        if(columnPos == 0){
            searchPos = 2;
        } else if(columnPos == 1){
            searchPos = 4;
        } else if(columnPos == 2){
            searchPos = 6;
        }
        gtk_tree_view_set_search_column(GTK_TREE_VIEW(data),searchPos);
    };
    g_signal_connect(m_searchIssueByComboBox,"changed",G_CALLBACK(searchIssueByChangedCallback),m_issuesTreeView);

    //
    //  When F5 key is pressed
    //
    GtkWidget* window = GTK_WIDGET(this->getViewManager()->getDefaultStage());
    gboolean (*keyPressCallback)(GtkWidget*,GdkEventKey*,gpointer) = [&](GtkWidget *widget,GdkEventKey *event,gpointer data){
        if(string(gdk_keyval_name(event->keyval)) == "F5"){
            g_message("MainController: Page Refresh Key Event");
            MainController* controller = static_cast<MainController*>(data);
            controller->loadTrees();
        }
        return FALSE;
    };
    g_signal_connect(window,"key-release-event",G_CALLBACK(keyPressCallback),this);
    g_message("MainController: Handled F5 Key Pressed callback");

    //
    //  When Add new Book button is clicked
    //
    GAppEventCallback addBookCallback = [&](GtkWidget* widget,gpointer data){
        g_message("MainController: Add Book button clicked");
        MainController* controller = static_cast<MainController*>(data);
        controller->saveBook(false);
    };
    g_signal_connect(m_addNewBookBtn,"clicked",G_CALLBACK(addBookCallback),this);
    g_message("MainController: Handled Add New Book button clicked callback");

    //
    //  When Edit Book button is clicked
    //
    GAppEventCallback editBookCallback = [&](GtkWidget* widget,gpointer data){
        g_message("MainController: Edit Book button clicked");
        MainController* controller = static_cast<MainController*>(data);
        controller->saveBook(true);
    };
    g_signal_connect(this->m_editBookBtn,"clicked",G_CALLBACK(editBookCallback),this);
    g_message("MainController: Handled Edit Book button clicked callback");

    //
    //  When Delete Book button is clicked
    //
    GAppEventCallback deleteBookCallBack = [&](GtkWidget* btn,gpointer data){
        MainController* controller = static_cast<MainController*>(data);
        GtkWidget* confirmDialog = controller->getSceneObj("deleteConfirmDialog");
        gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(confirmDialog),"<b>Are you sure want to delete this book?</b>");
        int rc = gtk_dialog_run(GTK_DIALOG(confirmDialog));

        if(rc == GTK_RESPONSE_YES){
            int bookId = controller->getSelectedBookIdFromTree();

            if(bookId != 0){
                if(BookCopy::hasIssuedForBook(bookId)){
                    GtkWidget* warningDialog = controller->getSceneObj("warningDialog");
                    gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(warningDialog),"<b>Some copies of this book are still issued! \n Return them first to delete this book</b>");
                    gtk_dialog_run(GTK_DIALOG(warningDialog));
                    gtk_widget_hide(warningDialog);
                } else {
                    DB::transaction([&](){
                        Book* book = Book::find<Book>(bookId);
                        book->deleteRow();
                        delete book;

                        vector<int> copyIds = BookCopy::getCopyIds(bookId);
                        Issue::deleteForCopies(copyIds);

                        BookCopy::deleteForBook(bookId);

                        controller->loadBooks();
                    });
                }
            }

        }
        gtk_widget_hide(confirmDialog);
    };
    g_signal_connect(m_deleteBookBtn,"clicked",G_CALLBACK(deleteBookCallBack),this);

    //
    //  When Add new User button is clicked
    //
    GAppEventCallback addUserCallback = [&](GtkWidget* widget,gpointer data){
        MainController* controller = static_cast<MainController*>(data);
        controller->saveUser(false);
    };
    g_signal_connect(m_addUserBtn,"clicked",G_CALLBACK(addUserCallback),this);

    //
    //  When Edit User Button is clicked
    //
    GAppEventCallback editUserCallback = [&](GtkWidget* widget,gpointer data){
        MainController* controller = static_cast<MainController*>(data);
        controller->saveUser(true);
    };
    g_signal_connect(m_editUserBtn,"clicked",G_CALLBACK(editUserCallback),this);

    //
    //  When Delete User Button is clicked
    //
    GAppEventCallback deleteUserCallback = [&](GtkWidget* widget,gpointer data){
        MainController* controller = static_cast<MainController*>(data);

        int userId = controller->getSelectedUserIdFromTree();
        if(userId != 0){
            GtkWidget* confirmDialog = controller->getSceneObj("deleteConfirmDialog");
            gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(confirmDialog),"<b>Are you sure to delete this user?</b>");
            int rc = gtk_dialog_run(GTK_DIALOG(confirmDialog));

            if(rc == GTK_RESPONSE_YES){
                if(Issue::hasIssuesForUser(userId)){
                    GtkWidget* warningDialog = controller->getSceneObj("warningDialog");
                    gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(warningDialog),"<b>This user has not returned some of the books. Make sure they are returned before deleting the user</b>");
                    gtk_dialog_run(GTK_DIALOG(warningDialog));
                    gtk_widget_hide(warningDialog);
                } else {
                    DB::transaction([&](){
                        User* user = User::find<User>(userId);
                        if(!user->deleteRow()){
                            throw exception();
                        }
                        Issue::deleteForUser(userId);
                        controller->loadUsers();
                        delete user;
                    });
                }
            }
            gtk_widget_hide(confirmDialog);
        }

    };
    g_signal_connect(m_deleteUserBtn,"clicked",G_CALLBACK(deleteUserCallback),this);

    //
    //  When Issue Book button is clicked
    //
    GAppEventCallback issueBookCallBack = [&](GtkWidget* widget,gpointer data){
        g_message("MainController: Issue Book Copy button clicked");
        MainController* controller = static_cast<MainController*>(data);
        int copyId = controller->getSelectedBookCopyIdFromTree();
        if(copyId != -1){
            Bundle* package = BundleFacade::get();
            package->putExtra("copyId",copyId);
            controller->startController("IssueBookController",package);
        }
    };
    g_signal_connect(this->m_issueBookCopyBtn,"clicked",G_CALLBACK(issueBookCallBack),this);
    g_message("MainController: Handled Issue Book button clicked callback");

    //
    //  When Renew Book button is clicked
    //
    GAppEventCallback renewBookCopyCallback = [&](GtkWidget* widget,gpointer data){
        MainController* controller = static_cast<MainController*>(data);
        int copyId = controller->getSelectedBookCopyIdFromTree();
        if(copyId != -1){
            Bundle* package = BundleFacade::get();
            package->putExtra("copyId",copyId);
            package->putExtra("renewMode",true);
            controller->startController("UnissueBookCopyController",package);
        }
    };
    g_signal_connect(this->m_renewBookCopyBtn,"clicked",G_CALLBACK(renewBookCopyCallback),this);

    //
    //  When Return Book button is clicked
    //
    GAppEventCallback returnBookCopyCallback = [&](GtkWidget* widget,gpointer data){
        MainController* controller = static_cast<MainController*>(data);
        int copyId = controller->getSelectedBookCopyIdFromTree();
        if(copyId != -1){
            Bundle* package = BundleFacade::get();
            package->putExtra("copyId",copyId);
            package->putExtra("renewMode",false);
            controller->startController("UnissueBookCopyController",package);
        }
    };
    g_signal_connect(this->m_returnBookCopyBtn,"clicked",G_CALLBACK(returnBookCopyCallback),this);

    //
    //  When Add Book Copy Button is clicked
    //
    GAppEventCallback addBookCopyCallback = [&](GtkWidget* widget,gpointer data){
        g_message("MainController: Add Book copy button clicked");
        MainController* controller = static_cast<MainController*>(data);
        controller->saveBookCopy(false);
    };
    g_signal_connect(m_addNewBookCopyBtn,"clicked",G_CALLBACK(addBookCopyCallback),this);

    //
    //  When Edit Book Copy Button is clicked
    //
    GAppEventCallback editBookCopyCallback = [&](GtkWidget* widget,gpointer data){
        g_message("MainController: Edit Book copy button clicked");
        MainController* controller = static_cast<MainController*>(data);
        controller->saveBookCopy(true);
    };

    //  @todo: Add for delete book button
    g_signal_connect(m_editBookCopyBtn,"clicked",G_CALLBACK(editBookCopyCallback),this);


    // delete book copy button
    GAppEventCallback deletebookcopyCallback=[&](GtkWidget* btn, gpointer data){
        MainController* controller = static_cast<MainController*>(data);
        GtkWidget* confirmDialog = controller->getSceneObj("deleteConfirmDialog");
        gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(confirmDialog), "<b>Are you sure to delete this book copy?</b>");
        int rc= gtk_dialog_run(GTK_DIALOG(confirmDialog));

        if(rc==GTK_RESPONSE_YES){
            int copyId = controller->getSelectedBookCopyIdFromTree();
            g_message("%i",copyId);
            if(copyId != 0){
                BookCopy* bookcopy = BookCopy::find<BookCopy>(copyId);
                bookcopy->deleteRow();
                controller->loadBookCopies(controller->getSelectedBookIdFromTree());
                delete bookcopy;
            }
        }
        gtk_widget_hide(confirmDialog);
    };
    g_signal_connect(m_deleteBookCopyBtn,"clicked",G_CALLBACK(deletebookcopyCallback),this);


    this->unsetUserOptionsSensitivity();
    this->unsetBookCopyOptionsSensitivity();
    this->normalizePermisions();

    //...if logged in
    if(Auth::isLoggedIn()){
        User* user = (User*)(Auth::getModel());

        string name = user->fetch()->getString("first_name") + " ";
        if(user->fetch()->hasString("middle_name")){
            name += user->fetch()->getString("middle_name") + string(" ");
        }
        name += user->fetch()->getString("last_name");

        gtk_label_set_text(GTK_LABEL(m_userNameLabel),name.c_str());
        gtk_button_set_label(GTK_BUTTON(m_logOutBtn),"Log Out");
    } else {
        gtk_button_set_label(GTK_BUTTON(m_logOutBtn),"Log In");
    }
}


void MainController::onStart(){
    Controller::onStart();
    if(!m_isFirstLoaded){
        m_isFirstLoaded = true;
        this->loadTrees();
    }
}

void MainController::onPause(){
    Controller::onPause();
    if(m_canFinish){
        finish();
    }
}

void MainController::onResume(Bundle* package){
    Controller::onResume(package);

    if(package != nullptr){
        if(package->hasBoolean("bookSaved") && package->hasInt("book_id")){
            this->loadBooks();
        } else if(package->hasBoolean("bookCopySaved") && package->hasInt("book_id")){
            this->loadBookCopies(package->getInt("book_id"));
        } else if(package->hasBoolean("userSaved") && package->hasInt("user_id")){
            this->loadUsers();
        } else if(package->hasBoolean("issuesChanged")){
            if(package->hasInt("book_id") && package->getInt("book_id") != 0){
                this->loadBookCopies(package->getInt("book_id"));
            }
            this->loadIssues();
        }
    }
}


void MainController::loadBooks(){
    //if(Auth::isLoggedIn() && AclRolePrivilege::authHasPermision(AclEntity::ENTITY_BOOK,AclAction::ACTION_READ)){
        gtk_list_store_clear(m_booksListStore);
        Book::getBookList([&](Statement* stmt,Bundle* data){
            appendBookFromStmt(stmt);
        });
    //}
}

void MainController::loadBookInfo(GtkTreeModel* treeModel,GtkTreeIter* iter){
    int bookId;
    char* bookName;
    char* isbn;
    char* edition;
    char* publication;
    char* addedAt;
    gtk_tree_model_get(treeModel,iter,
                                0,&bookId,
                                1,&bookName,
                                2,&isbn,
                                3,&edition,
                                4,&publication,
                                5,&addedAt,-1);
    gtk_label_set_text(GTK_LABEL(this->m_bookIdLabel),Strings::toString(bookId).c_str());
    gtk_label_set_text(GTK_LABEL(this->m_bookNameLabel),bookName);
    gtk_label_set_text(GTK_LABEL(this->m_isbnLabel),isbn);
    gtk_label_set_text(GTK_LABEL(this->m_editionLabel),edition);
    gtk_label_set_text(GTK_LABEL(this->m_publicationLabel),publication);
    gtk_label_set_text(GTK_LABEL(this->m_addedAtLabel),addedAt);

    this->loadAuthors(bookId);
    this->loadBookCopies(bookId);
}
void MainController::loadAuthors(int book_id){
    vector<string> authors;
    BookAuthor::getAuthorsFromBook(book_id,[&](Statement* stmt,Bundle* data){
        string name = stmt->getString("first_name") + string(" ") + stmt->getString("middle_name") + string(" ") + stmt->getString("last_name");
        authors.push_back(name);
    });

    gtk_label_set_text(GTK_LABEL(this->m_authorsLabel),Strings::join(authors,"\n").c_str());
}
void MainController::loadBookCopies(int book_id){
    GtkTreeIter iter;
    gtk_list_store_clear(this->m_bookCopiesListStore);
    BookCopy::getCopiesForBook(book_id,[&](Statement* stmt,Bundle* data){
        gtk_list_store_append(this->m_bookCopiesListStore,&iter);
        gtk_list_store_set(this->m_bookCopiesListStore,&iter,
                           0,stmt->getInt("copy_id"),
                           1,stmt->getString("copy_code").c_str(),
                           2,stmt->getInt("status_id"),
                           3,BookCopy::getStatusString(stmt->getInt("status_id")).c_str(),-1);
    });

   unsetBookCopyOptionsSensitivity();
}

void MainController::loadIssues(){
    gtk_list_store_clear(m_issuesListStore);

    GtkTreeIter iter;
    Issue::getIssues([&](Statement* stmt,Bundle* data){
        int statusId = stmt->getInt("status_id");
        string userName = Strings::join(vector<string>({stmt->getString("first_name"),stmt->getString("middle_name"),stmt->getString("last_name")})," ");

        gtk_list_store_append(m_issuesListStore,&iter);
        gtk_list_store_set(m_issuesListStore,&iter,
                            0,stmt->getInt("issue_id"),
                            1,stmt->getInt("book_id"),
                            2,stmt->getString("book_name").c_str(),
                            3,stmt->getInt("copy_id"),
                            4,stmt->getString("copy_code").c_str(),
                            5,stmt->getInt("user_id"),
                            6,userName.c_str(),
                            7,stmt->getString("issued_at").c_str(),
                            8,stmt->getString("deadline_at").c_str(),
                            9,statusId,
                            10,BookCopy::getStatusString(statusId).c_str(),-1);
    });
}

void MainController::loadUsers(){
    gtk_list_store_clear(m_usersListStore);
    User::getValidUsers([&](Statement* stmt,Bundle* data){
        appendUserFromStmt(stmt);
    });

    this->normalizeUserOptions(gtk_tree_view_get_selection(GTK_TREE_VIEW(m_usersTreeView)));
}

void MainController::loadTrees(){
    this->loadBooks();
    this->loadIssues();
    this->loadUsers();
}

void MainController::appendBookFromStmt(Statement* stmt){
    gtk_list_store_append(m_booksListStore,&m_bookIterator);
    setBookFromStmt(stmt,&m_bookIterator);
}

void MainController::setBookFromStmt(Statement* stmt,GtkTreeIter* iter){
    string edition = Strings::toString(stmt->getInt("edition")) + string(" Edition");
    gtk_list_store_set(m_booksListStore,iter,
                           0,stmt->getInt("book_id"),
                           1,stmt->getString("book_name").c_str(),
                           2,stmt->getString("isbn").c_str(),
                           3,edition.c_str(),
                           4,stmt->getString("publication_name").c_str(),
                           5,stmt->getString("created_at").c_str(),-1);
}

void MainController::appendUserFromStmt(Statement* stmt){
    gtk_list_store_append(m_usersListStore,&m_userIterator);
    setUserFromStmt(stmt,&m_userIterator);
}

void MainController::setUserFromStmt(Statement* stmt,GtkTreeIter* iter){
    string name = Strings::join(vector<string>({stmt->getString("first_name"),stmt->getString("middle_name"),stmt->getString("last_name")})," ");
    gtk_list_store_set(m_usersListStore,iter,
                           0,stmt->getInt("user_id"),
                           1,name.c_str(),
                           2,stmt->getInt("role_id"),
                           3,stmt->getString("role_name").c_str(),
                           4,stmt->getInt("status_id"),
                           5,"Valid",-1);
}


void MainController::unsetBookCopyOptionsSensitivity(){
    gtk_widget_set_sensitive(m_editBookCopyBtn,false);
    gtk_widget_set_sensitive(m_deleteBookCopyBtn,false);
    gtk_widget_set_sensitive(m_issueBookCopyBtn,false);
    gtk_widget_set_sensitive(m_renewBookCopyBtn,false);
    gtk_widget_set_sensitive(m_returnBookCopyBtn,false);
}

void MainController::normalizeBookCopyOptions(GtkTreeSelection* selection){
    unsetBookCopyOptionsSensitivity();

    GtkTreeIter iter;
    int statusId;
    GtkTreeModel* copiesModel = gtk_tree_view_get_model(GTK_TREE_VIEW(m_bookCopiesTreeView));
    if(gtk_tree_selection_get_selected(selection,&copiesModel,&iter)){
        gtk_tree_model_get(copiesModel,&iter,2,&statusId,-1);

        gtk_widget_set_sensitive(m_editBookCopyBtn,true);
        gtk_widget_set_sensitive(m_deleteBookCopyBtn,true);

        if(statusId == BookCopy::STATUS_ISSUED){
            gtk_widget_set_sensitive(m_renewBookCopyBtn,true);
            gtk_widget_set_sensitive(m_returnBookCopyBtn,true);
            gtk_widget_set_sensitive(m_deleteBookCopyBtn,false);
        } else if (statusId == BookCopy::STATUS_ISSUEABLE){
            gtk_widget_set_sensitive(m_issueBookCopyBtn,true);
            gtk_widget_set_sensitive(m_deleteBookCopyBtn,true);
        }
    }
}

void MainController::unsetUserOptionsSensitivity(){
    gtk_widget_set_sensitive(m_editUserBtn,false);
    gtk_widget_set_sensitive(m_deleteUserBtn,false);
}

void MainController::normalizeUserOptions(GtkTreeSelection* selection){
    unsetUserOptionsSensitivity();

    GtkTreeIter iter;
    GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(m_usersTreeView));
    if(gtk_tree_selection_get_selected(selection,&model,&iter)){
        gtk_widget_set_sensitive(m_editUserBtn,true);

        int userId = this->getSelectedUserIdFromTree();
        if(userId != 0){
            if(Auth::isLoggedIn() && userId != Auth::id()){
                gtk_widget_set_sensitive(m_deleteUserBtn,true);
            }
        }
    }
}

int MainController::getSelectedBookIdFromTree(){
    int bookId = -1;
    GtkTreeIter iter;
    GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(m_booksTreeView));
    GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(m_booksTreeView));
    if(gtk_tree_selection_get_selected(selection,&model,&iter)){
        gtk_tree_model_get(model,&iter,0,&bookId,-1);
    }
    return bookId;
}

int MainController::getSelectedBookCopyIdFromTree(){
    int copyId = -1;
    GtkTreeIter iter;
    GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(m_bookCopiesTreeView));
    GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(m_bookCopiesTreeView));
    if(gtk_tree_selection_get_selected(selection,&model,&iter)){
        gtk_tree_model_get(model,&iter,0,&copyId,-1);
    }
    return copyId;
}

int MainController::getSelectedUserIdFromTree(){
    int userId = -1;
    GtkTreeIter iter;
    GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(m_usersTreeView));
    GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(m_usersTreeView));
    if(gtk_tree_selection_get_selected(selection,&model,&iter)){
        gtk_tree_model_get(model,&iter,0,&userId,-1);
    }
    return userId;
}

string MainController::getSelectedBookPath(){
    return Gtk::getSelectedTreePath(GTK_TREE_VIEW(m_booksTreeView));
}

string MainController::getSelectedBookCopyPath(){
    return Gtk::getSelectedTreePath(GTK_TREE_VIEW(m_bookCopiesTreeView));
}

string MainController::getSelectedUserPath(){
    return Gtk::getSelectedTreePath(GTK_TREE_VIEW(m_usersTreeView));
}

void MainController::saveBook(bool editMode){
    Bundle* package = nullptr;
    if(editMode){
        int bookId = atoi(gtk_label_get_text(GTK_LABEL(m_bookIdLabel)));
        string path = getSelectedBookPath();
        if(path == ""){
            return;
        }

        g_message("MainController: Book of [%i] id to be saved",bookId);

        package = BundleFacade::get();
        package->putExtra("book_id",bookId)
                ->putExtra("selectedBookPath",path);
    }

    startController("SaveBookController",package);
}

void MainController::saveBookCopy(bool editMode){
    Bundle* package = BundleFacade::get();
    package->putExtra("book_id",atoi(gtk_label_get_text(GTK_LABEL(m_bookIdLabel))));
    if(editMode){
        int copyId = getSelectedBookCopyIdFromTree();
        string path = getSelectedBookCopyPath();
        if(copyId == -1 && path == ""){
            return;
        }
        package->putExtra("copy_id",copyId)
                ->putExtra("selectedBookCopyPath",path);
    }
    g_message("Starting SaveBookCopiesController");
    startController("SaveBookCopiesController",package);
}


void MainController::saveUser(bool editMode){
    Bundle* package = nullptr;
    if(editMode){
        int userId = this->getSelectedUserIdFromTree();
        string path = getSelectedUserPath();
        if(userId == -1 && path == ""){
            return;
        }
        package = BundleFacade::get();
        package->putExtra("user_id",userId)
                ->putExtra("selectedUserPath",path);
    }
    startController("SaveUserController",package);
}

void MainController::normalizePermisions(){
    if(Auth::isLoggedIn()){
        int pagePos = 0;
        if(!AclRolePrivilege::authHasPermision(AclEntity::ENTITY_BOOK,AclAction::ACTION_READ)){
            gtk_notebook_remove_page(GTK_NOTEBOOK(m_mainNotebook),0);
            pagePos--;
        }
        if(!AclRolePrivilege::authHasPermision(AclEntity::ENTITY_ISSUE,AclAction::ACTION_READ)){
            gtk_notebook_remove_page(GTK_NOTEBOOK(m_mainNotebook),pagePos + 1);
            pagePos--;
        }
        if(!AclRolePrivilege::authHasPermision(AclEntity::ENTITY_USER,AclAction::ACTION_READ)){
            gtk_notebook_remove_page(GTK_NOTEBOOK(m_mainNotebook),pagePos + 2);
            pagePos--;
        }

        if(!AclRolePrivilege::authHasPermision(AclEntity::ENTITY_BOOK,AclAction::ACTION_CREATE)){
            gtk_widget_hide(m_addNewBookBtn);
        }
        if(!AclRolePrivilege::authHasPermision(AclEntity::ENTITY_BOOK,AclAction::ACTION_UPDATE)){
            gtk_widget_hide(m_editBookBtn);
        }
        if(!AclRolePrivilege::authHasPermision(AclEntity::ENTITY_BOOK,AclAction::ACTION_DELETE)){
            gtk_widget_hide(m_deleteBookBtn);
        }

        if(!AclRolePrivilege::authHasPermision(AclEntity::ENTITY_BOOK_COPY,AclAction::ACTION_CREATE)){
            gtk_widget_hide(m_addNewBookCopyBtn);
        }
        if(!AclRolePrivilege::authHasPermision(AclEntity::ENTITY_BOOK_COPY,AclAction::ACTION_UPDATE)){
            gtk_widget_hide(m_editBookCopyBtn);
        }
        if(!AclRolePrivilege::authHasPermision(AclEntity::ENTITY_BOOK_COPY,AclAction::ACTION_DELETE)){
            gtk_widget_hide(m_deleteBookCopyBtn);
        }

        if(!AclRolePrivilege::authHasPermision(AclEntity::ENTITY_ISSUE,AclAction::ACTION_CREATE)){
            gtk_widget_hide(m_issueBookCopyBtn);
        }

        if(!AclRolePrivilege::authHasPermision(AclEntity::ENTITY_ISSUE,AclAction::ACTION_UPDATE)){
            gtk_widget_hide(m_returnBookCopyBtn);
            gtk_widget_hide(m_renewBookCopyBtn);
        }

        if(!AclRolePrivilege::authHasPermision(AclEntity::ENTITY_USER,AclAction::ACTION_CREATE)){
            gtk_widget_hide(m_addUserBtn);
        }
        if(!AclRolePrivilege::authHasPermision(AclEntity::ENTITY_USER,AclAction::ACTION_UPDATE)){
            gtk_widget_hide(m_editUserBtn);
        }
        if(!AclRolePrivilege::authHasPermision(AclEntity::ENTITY_USER,AclAction::ACTION_DELETE)){
            gtk_widget_hide(m_deleteUserBtn);
        }
    }
}
