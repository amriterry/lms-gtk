#include <cstdlib>
#include <list>
#include <algorithm>

#include "SaveBookController.h"

#include <system/bundle/Bundle.h>
#include <models/Book.h>
#include <models/Author.h>
#include <models/Publication.h>
#include <models/BookAuthor.h>
#include <models/BookCopy.h>
#include <models/AclEntity.h>
#include <models/AclAction.h>
#include <models/AclRolePrivilege.h>

void SaveBookController::onCreate(Bundle* package){
    Controller::onCreate(package);
    setTopLevelView("saveview.glade");

    this->m_bookName = getSceneObj("book_name");
    this->m_copyCount = getSceneObj("copyCount");
    this->m_isbn = getSceneObj("isbn");
    this->m_edition = getSceneObj("edition");

    this->m_copyCodeContainer = getSceneObj("copyCodeContainer");

    this->m_publicationComboBox = GTK_COMBO_BOX(getSceneObj("publicationComboBox"));

    this->m_authorsTreeView = getSceneObj("authorsTreeView");
    this->m_selectedAuthorsTreeView = getSceneObj("selectedAuthorsTreeView");

    this->m_authorsListStore = GTK_LIST_STORE(getSceneRawObj("authorsListStore"));
    this->m_selectedAuthorsListStore = GTK_LIST_STORE(getSceneRawObj("selectedAuthorsListStore"));
    this->m_publicationListStore = GTK_LIST_STORE(getSceneRawObj("publicationListStore"));

    this->m_addPublicationBtn = getSceneObj("addPublicationBtn");
    this->m_editPublicationBtn = getSceneObj("editPublicationBtn");
    this->m_deletePublicationBtn = getSceneObj("deletePublicationBtn");

    this->m_addAuthorBtn = getSceneObj("addAuthorBtn");
    this->m_editAuthorBtn = getSceneObj("editAuthorBtn");
    this->m_deleteAuthorBtn = getSceneObj("deleteAuthorBtn");

    this->m_addAuthorSelectionBtn = getSceneObj("addAuthorSelectionBtn");
    this->m_removeAuthorSelectionBtn = getSceneObj("removeAuthorSelectionBtn");

    GtkTreeSelection* authorsSelection = gtk_tree_view_get_selection(GTK_TREE_VIEW(m_authorsTreeView));
    GtkTreeSelection* selectedAuthorsSelection = gtk_tree_view_get_selection(GTK_TREE_VIEW(m_selectedAuthorsTreeView));

    gtk_tree_selection_set_mode(authorsSelection,GTK_SELECTION_MULTIPLE);
    gtk_tree_selection_set_mode(selectedAuthorsSelection,GTK_SELECTION_MULTIPLE);

    void (*authorsSelectionChanged)(GtkTreeSelection *treeselection,gpointer user_data) = [&](GtkTreeSelection *treeselection,gpointer user_data){
        Controller* controller = static_cast<Controller*>(user_data);

        int countAuthors = gtk_tree_selection_count_selected_rows(treeselection);

        GtkWidget* editAuthorBtn = controller->getSceneObj("editAuthorBtn");
        GtkWidget* deleteAuthorBtn = controller->getSceneObj("deleteAuthorBtn");
        GtkWidget* addAuthorSelectionBtn = controller->getSceneObj("addAuthorSelectionBtn");
        GtkWidget* removeAuthorsSelectionBtn = controller->getSceneObj("removeAuthorSelectionBtn");

        gtk_widget_set_sensitive(editAuthorBtn,false);
        gtk_widget_set_sensitive(deleteAuthorBtn,false);
        gtk_widget_set_sensitive(addAuthorSelectionBtn,false);
        gtk_widget_set_sensitive(removeAuthorsSelectionBtn,false);

        if(countAuthors > 0){
            gtk_widget_set_sensitive(addAuthorSelectionBtn,true);
        }

        if(countAuthors == 1){
            gtk_widget_set_sensitive(editAuthorBtn,true);
            gtk_widget_set_sensitive(deleteAuthorBtn,true);
        }

        GtkTreeSelection* sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(controller->getSceneObj("selectedAuthorsTreeView")));
        gtk_tree_selection_unselect_all(sel);
    };
    g_signal_connect(authorsSelection,"changed",G_CALLBACK(authorsSelectionChanged),this);

    void (*selectedAuthorsSelectionChanged)(GtkTreeSelection *treeselection,gpointer user_data) = [&](GtkTreeSelection *treeselection,gpointer user_data){
        Controller* controller = static_cast<Controller*>(user_data);
        int countAuthors = gtk_tree_selection_count_selected_rows(treeselection);

        GtkWidget* editAuthorBtn = controller->getSceneObj("editAuthorBtn");
        GtkWidget* deleteAuthorBtn = controller->getSceneObj("deleteAuthorBtn");
        GtkWidget* addAuthorSelectionBtn = controller->getSceneObj("addAuthorSelectionBtn");
        GtkWidget* removeAuthorsSelectionBtn = controller->getSceneObj("removeAuthorSelectionBtn");

        gtk_widget_set_sensitive(editAuthorBtn,false);
        gtk_widget_set_sensitive(deleteAuthorBtn,false);
        gtk_widget_set_sensitive(addAuthorSelectionBtn,false);
        gtk_widget_set_sensitive(removeAuthorsSelectionBtn,false);

        if(countAuthors > 0){
            gtk_widget_set_sensitive(removeAuthorsSelectionBtn,true);
        }

        if(countAuthors == 1){
            gtk_widget_set_sensitive(editAuthorBtn,true);
            gtk_widget_set_sensitive(deleteAuthorBtn,true);
        }

         GtkTreeSelection* sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(controller->getSceneObj("authorsTreeView")));
        gtk_tree_selection_unselect_all(sel);
    };
    g_signal_connect(selectedAuthorsSelection,"changed",G_CALLBACK(selectedAuthorsSelectionChanged),this);

    void (*copyCountChangedCallback)(GtkSpinButton*,gpointer) = [&](GtkSpinButton* spinner,gpointer data){
        g_message("Value Changed");
        Controller* controller = static_cast<Controller*>(data);
        int copyCount = gtk_spin_button_get_value_as_int(spinner);
        GtkWidget* box = controller->getSceneObj("copyCodeContainer");
        GList* entries = gtk_container_get_children(GTK_CONTAINER(box));
        int numEntries = g_list_length(entries);
        g_message("Copy Count: %i ; Num Entries: %i",copyCount,numEntries);
        if(copyCount > numEntries){
            for(int i = numEntries;i < copyCount; i++){
                int entryId = i + 1;
                GtkWidget* entry = gtk_entry_new();
                string name = string("copyCodeEntry") + Strings::toString(entryId);
                g_message("Name: %s",name.c_str());
                gtk_widget_set_name(entry,name.c_str());
                gtk_box_pack_start(GTK_BOX(box),entry,0,0,0);
                gtk_widget_show(entry);
            }
        } else  if(copyCount < numEntries){
            GList* extraEntries = g_list_nth(entries,copyCount);
            while(extraEntries != NULL){
                gtk_widget_destroy(GTK_WIDGET(extraEntries->data));
                extraEntries = extraEntries->next;
            }
        }
    };
    g_signal_connect(m_copyCount,"value-changed",G_CALLBACK(copyCountChangedCallback),this);

    //  Connecting addAuthorSelectionBtn to a callback
    //  This moves selected author from authorsList to selectedAuthorsList
    GAppEventCallback addAuthorSelectionCallback = [&](GtkWidget* btn,gpointer data){
        SaveBookController* controller = static_cast<SaveBookController*>(data);
        GtkTreeView* authorsTreeView = GTK_TREE_VIEW(controller->getSceneObj("authorsTreeView"));
        GtkTreeView* selectedAuthorsTreeView = GTK_TREE_VIEW(controller->getSceneObj("selectedAuthorsTreeView"));

        GtkListStore* authorsListStore = GTK_LIST_STORE(controller->getSceneRawObj("authorsListStore"));
        GtkListStore* selectedAuthorsListStore = GTK_LIST_STORE(controller->getSceneRawObj("selectedAuthorsListStore"));

        controller->moveSelectedAuthors(authorsTreeView,authorsListStore,selectedAuthorsTreeView,selectedAuthorsListStore);
    };
    g_signal_connect(m_addAuthorSelectionBtn,"clicked",G_CALLBACK(addAuthorSelectionCallback),this);

    GAppEventCallback removeAuthorSelectionCallback = [&](GtkWidget* btn,gpointer data){
        SaveBookController* controller = static_cast<SaveBookController*>(data);
        GtkTreeView* authorsTreeView = GTK_TREE_VIEW(controller->getSceneObj("authorsTreeView"));
        GtkTreeView* selectedAuthorsTreeView = GTK_TREE_VIEW(controller->getSceneObj("selectedAuthorsTreeView"));

        GtkListStore* authorsListStore = GTK_LIST_STORE(controller->getSceneRawObj("authorsListStore"));
        GtkListStore* selectedAuthorsListStore = GTK_LIST_STORE(controller->getSceneRawObj("selectedAuthorsListStore"));

        controller->moveSelectedAuthors(selectedAuthorsTreeView,selectedAuthorsListStore,authorsTreeView,authorsListStore);
    };
    g_signal_connect(m_removeAuthorSelectionBtn,"clicked",G_CALLBACK(removeAuthorSelectionCallback),this);


    GAppEventCallback addPublicationCallback = [&](GtkWidget* btn,gpointer data){
        SaveBookController* controller = static_cast<SaveBookController*>(data);
        controller->savePublication();
    };
    g_signal_connect(m_addPublicationBtn,"clicked",G_CALLBACK(addPublicationCallback),this);

    GAppEventCallback editPublicationCallback = [&](GtkWidget* btn,gpointer data){
        SaveBookController* controller = static_cast<SaveBookController*>(data);
        controller->savePublication(true);
    };
    g_signal_connect(m_editPublicationBtn,"clicked",G_CALLBACK(editPublicationCallback),this);

    //...delete publication btn event
    GAppEventCallback deletePublicationCallback = [&](GtkWidget* btn,gpointer data){
        SaveBookController* controller = static_cast<SaveBookController*>(data);
        GtkWidget* confirmDialog = controller->getSceneObj("deleteConfirmDialog");
        gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(confirmDialog),"<b>Are you sure to delete this publisher?</b>");
        int rc = gtk_dialog_run(GTK_DIALOG(confirmDialog));

        if(rc == GTK_RESPONSE_YES){
            GtkComboBox* box = GTK_COMBO_BOX(controller->getSceneObj("publicationComboBox"));
            int publication_id = atoi(gtk_combo_box_get_active_id(box));
            if(Book::hasForPublication(publication_id)){
                GtkWidget* errorDialog = controller->getSceneObj("errorDialog");
                gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(errorDialog),"<b>Other Books are using this publication. Change the publication of other books to delete this publication</b>");
                gtk_dialog_run(GTK_DIALOG(errorDialog));
                gtk_widget_hide(errorDialog);
            } else {
                Publication* pub = Publication::find<Publication>(publication_id);
                pub->deleteRow();
                delete pub;

                controller->loadPublications();

                //...refresh the publication List
            }
        }

        gtk_widget_hide(confirmDialog);
    };
    g_signal_connect(m_deletePublicationBtn,"clicked",G_CALLBACK(deletePublicationCallback),this);

    GAppEventCallback addAuthorCallback = [&](GtkWidget* btn,gpointer data){
        SaveBookController* controller = static_cast<SaveBookController*>(data);
        controller->saveAuthor();
    };
    g_signal_connect(m_addAuthorBtn,"clicked",G_CALLBACK(addAuthorCallback),this);

    GAppEventCallback editAuthorCallback = [&](GtkWidget* btn,gpointer data){
        SaveBookController* controller = static_cast<SaveBookController*>(data);
        controller->saveAuthor(true);
    };
    g_signal_connect(m_editAuthorBtn,"clicked",G_CALLBACK(editAuthorCallback),this);

    //..delete author button event
    GAppEventCallback deleteAuthorCallback = [&](GtkWidget* btn,gpointer data){
        SaveBookController* controller = static_cast<SaveBookController*>(data);
        GtkWidget* confirmDialog = controller->getSceneObj("deleteConfirmDialog");
        gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(confirmDialog),"<b>Are you sure to delete author?</b>");
        int rc = gtk_dialog_run(GTK_DIALOG(confirmDialog));

        if (rc == GTK_RESPONSE_YES){
            int authorId = controller->getSelectedAuthorId();
            if(authorId != 0){
                if(BookAuthor::isAuthorBinded(authorId)){
                    GtkWidget* errorDialog = controller->getSceneObj("errorDialog");
                    gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(errorDialog),"<b>Other Books are using this author. Remove this author from the book to delete this author</b>");
                    gtk_dialog_run(GTK_DIALOG(errorDialog));
                    gtk_widget_hide(errorDialog);
                } else {
                    Author* author = Author::find<Author>(authorId);
                    author->deleteRow();
                    delete author;

                    controller->loadAuthors();

                    //...refresh the author List
                }
            }
        }

        gtk_widget_hide(confirmDialog);
    };
    g_signal_connect(m_deleteAuthorBtn,"clicked",G_CALLBACK(deleteAuthorCallback),this);


    if(package != nullptr){
        if(package->hasInt("book_id")){
            this->m_book = Book::find<Book>(package->getInt("book_id"));
            gtk_entry_set_text(GTK_ENTRY(m_bookName),m_book->fetch()->getString("book_name").c_str());
            gtk_entry_set_text(GTK_ENTRY(m_isbn),m_book->fetch()->getString("isbn").c_str());
            gtk_entry_set_text(GTK_ENTRY(m_edition),Strings::toString(m_book->fetch()->getInt("edition")).c_str());
            gtk_widget_hide(getSceneObj("copyNumBox"));

            this->m_editMode = true;

            g_message("\n\n****Book Id: [%i]****\n\n",package->getInt("book_id"));
        }

        if(package->hasString("selectedBookPath")){
            this->m_selectedBookPath = package->getString("selectedBookPath");
        }
    } else {
        this->m_book = new Book();
    }
}

void SaveBookController::onStart(){
    Controller::onStart();
    clock_t begin = clock();

    this->loadAuthors();

    this->loadPublications();

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    g_message("SaveBookController: onStart completed at: %f",elapsed_secs);

    int rc;
    Bundle* returnPackage = nullptr;

    g_message("SaveBookController: Dialog Going to run");
    while((rc = gtk_dialog_run(GTK_DIALOG(getSceneRoot()))) == GTK_RESPONSE_OK){
        g_message("SaveBookController: Dialog ran");
        string bookName = gtk_entry_get_text(GTK_ENTRY(m_bookName));
        string isbn = gtk_entry_get_text(GTK_ENTRY(m_isbn));
        int copyCount = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(m_copyCount));
        int publicationId = atoi(gtk_combo_box_get_active_id(m_publicationComboBox));
        int edition = atoi(gtk_entry_get_text(GTK_ENTRY(m_edition)));

        vector<int> authors = this->getSelectedAuthorsList();

        vector<string> errors;

        if(bookName == ""){
            errors.push_back("<b>Book Name Cannot be empty</b>");
        }
        if(isbn == ""){
            errors.push_back("<b>Isbn Number Cannot be empty</b>");
        }
        if(m_editMode == false && copyCount == 0){
            errors.push_back("<b>A book should have atleast 1 copy</b>");
        }
        if(publicationId < 1){
            errors.push_back("<b>A should have a publication</b>");
        }
        if(edition <= 0){
            errors.push_back("<b>Book edition should be a positive number</b>");
        }
        if(authors.empty()){
            errors.push_back("<b>A Book should have at least one author</b>");
        }

        vector<string> copyCodes;
        if(!m_editMode){
            GList* children = gtk_container_get_children(GTK_CONTAINER(m_copyCodeContainer));
            while(children != NULL){
                GtkEntry* entry = GTK_ENTRY(children->data);
                string code = gtk_entry_get_text(entry);
                if(code == ""){
                    errors.push_back("<b>Copy Code Should Not be empty</b>");
                    break;
                } else {
                    copyCodes.push_back(code);
                }
                children = children->next;
            }
        }

        g_message("%s",Strings::join(errors,"\n").c_str());

        if(!errors.empty()){
            GtkWidget* errorDialog = getSceneObj("errorDialog");
            gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(errorDialog),Strings::join(errors,"\n").c_str());
            gtk_dialog_run(GTK_DIALOG(errorDialog));
            gtk_widget_hide(errorDialog);
            continue;
        }

        DB::transaction([&](){
            this->m_book->getDirty()
                    ->putExtra("book_name",bookName)
                    ->putExtra("isbn",isbn)
                    ->putExtra("publication_id",publicationId)
                    ->putExtra("edition",edition);

            if(!m_editMode){
                this->m_book->getDirty()->putExtra("status_id",1);
            }

            if(!this->m_book->saveRow()){
                throw exception();
            }

            int bookId = m_book->fetch()->getInt("book_id");
            g_message("AfterSaved: \n\n\n\n\n[%i]\n\n\n\n",bookId);

            if(!m_editMode){
                BookCopy* bookCopy = new BookCopy();
                for(auto copyCode: copyCodes){
                    bookCopy->getDirty()
                            ->putExtra("book_id",bookId)
                            ->putExtra("copy_code",copyCode)
                            ->putExtra("status_id",BookCopy::STATUS_ISSUEABLE);

                    if(!bookCopy->saveRow()){
                        throw exception();
                        break;
                    }
                }
                delete bookCopy;
            }

            BookAuthor* bookAuthor = new BookAuthor();
            bookAuthor->deleteForBook(bookId);
            for(auto authorId: authors){
                bookAuthor->getDirty()
                        ->putExtra("book_id",bookId)
                        ->putExtra("author_id",authorId);

                if(!bookAuthor->saveRow()){
                    throw exception();
                }
            }

            delete bookAuthor;
        });

        //Event::fireIfExistsAndForget("Book.Save");
        if(returnPackage == nullptr){
            returnPackage = BundleFacade::get();
        }
        returnPackage->putExtra("book_id",this->m_book->fetch()->getInt("book_id"))
                    ->putExtra("bookSaved",true);
        if(m_selectedBookPath != ""){
            returnPackage->putExtra("selectedBookPath",m_selectedBookPath);
        }
        break;
    }

    g_message("Response Id: %d",rc);

    finish(returnPackage);
}

void SaveBookController::onPause(){
    Controller::onPause();
}

void SaveBookController::onStop(){
    Controller::onStop();
    delete m_book;
}

void SaveBookController::moveSelectedAuthors(GtkTreeView* fromView,GtkListStore* fromStore,GtkTreeView* toView,GtkListStore* toStore){
    GtkTreeSelection* selection = gtk_tree_view_get_selection(fromView);
    GtkTreeModel* treeModel = gtk_tree_view_get_model(fromView);

    GList* selected = gtk_tree_selection_get_selected_rows(selection,&treeModel);

    int numSelected = g_list_length(selected);

    GtkTreeIter* selectedIters = (GtkTreeIter*) g_malloc(numSelected * sizeof(GtkTreeIter));

    int count = 0;
    int authorId;
    char* authorName;

    while(selected != NULL){
        GtkTreeIter fromIter;
        GtkTreeIter toIter;

        GtkTreePath* path = (GtkTreePath*) selected->data;
        if(gtk_tree_model_get_iter(treeModel,&fromIter,path)){
            gtk_tree_model_get(treeModel,&fromIter,0,&authorId,1,&authorName,-1);
            selectedIters[count] = fromIter;
            gtk_list_store_append(toStore,&toIter);
            gtk_list_store_set(toStore,&toIter,0,authorId,1,authorName,-1);
        }

        selected = selected->next;
        count++;
    }

    for(count = 0;count < numSelected;count++){
        gtk_list_store_remove(fromStore,&(selectedIters[count]));
    }

     g_free(selectedIters);
}

void SaveBookController::savePublication(bool editMode){
    Request* request = RequestFacade::get();
    request->setActionKey("SavePublicationController");
    if(editMode){
        int publicatonId = atoi(gtk_combo_box_get_active_id(m_publicationComboBox));

        Bundle* package = BundleFacade::get();
        package->putExtra("publication_id",publicatonId);
        request->carryPackage(package);
    }
    startController(request);
}

void SaveBookController::saveAuthor(bool editMode){
    Request* request = RequestFacade::get();
    request->setActionKey("SaveAuthorController");
    if(editMode){
        //int authorId = atoi(gtk_combo_box_get_active_id(m_author));

        int authorId  = getSelectedAuthorId();

        g_message("AuthorId: %i",authorId);

        if(authorId != 0){
            Bundle* package = BundleFacade::get();
            package->putExtra("author_id",authorId);
            request->carryPackage(package);
        }
    }
    startController(request);
}

void SaveBookController::loadCopyCodes(int bookId){
    int count = 1;
    BookCopy::getCopiesForBook(bookId,[&](Statement* stmt,Bundle* data){
        GtkWidget* entry;
        if(count == 1){
            g_message("SaveBookController: First Copy COde");
            entry = getSceneObj("copyCodeEntry1");
        } else {
            g_message("SaveBookController: copy count %i",count);
            entry = gtk_entry_new();
            string name = string("copyCodeEntry") + Strings::toString(count);
            gtk_widget_set_name(entry,name.c_str());
            gtk_box_pack_start(GTK_BOX(getSceneObj("copyCodeContainer")),entry,0,0,0);
        }
        gtk_entry_set_text(GTK_ENTRY(entry),stmt->getString("copy_code").c_str());
        gtk_widget_show(entry);
        count++;
    });
    count--;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(m_copyCount),count);
}

vector<int> SaveBookController::getSelectedAuthorsList(){
    g_message("SaveBookController: getting selected authors list");
    GtkTreeIter iter;
    int id;
    vector<int> result;
    GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(m_selectedAuthorsTreeView));
    if(gtk_tree_model_get_iter_first(model,&iter)){
        while(true){
            gtk_tree_model_get(model,&iter,0,&id,-1);
            result.push_back(id);
            if(!gtk_tree_model_iter_next(model,&iter)){
                break;
            }
        }
    }

    return result;
}

int SaveBookController::getSelectedAuthorId(){
    int authorId = 0;
    GtkTreeModel* model;
    GtkTreeIter iter;

    GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(m_authorsTreeView));
    if(gtk_tree_selection_count_selected_rows(selection)){
        model = gtk_tree_view_get_model(GTK_TREE_VIEW(m_authorsTreeView));
    } else {
        selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(m_selectedAuthorsTreeView));
        if(gtk_tree_selection_count_selected_rows(selection)){
            model = gtk_tree_view_get_model(GTK_TREE_VIEW(m_selectedAuthorsTreeView));
        }
    }

    GList* selectedList = gtk_tree_selection_get_selected_rows(selection,&model);
    GtkTreePath* path = (GtkTreePath*) (selectedList->data);
    gtk_tree_model_get_iter(model,&iter,path);

    gtk_tree_model_get(model,&iter,0,&authorId,-1);

    return authorId;
}

void SaveBookController::loadAuthors(){
    GtkTreeIter authorIter;
    GtkTreeIter selectedAuthorIter;
    list<int> selectedAuthors;

    if(m_book->exists()){
        selectedAuthors = BookAuthor::getAuthorListFromBook(m_book->fetch()->getInt("book_id"));
    }

    gtk_list_store_clear(m_authorsListStore);
    gtk_list_store_clear(m_selectedAuthorsListStore);
    Author::all<Author>([&](Statement* stmt,Bundle* data){
        int authorId = stmt->getInt("author_id");
        string name = stmt->getString("first_name") + string(" ") + stmt->getString("middle_name") + string(" ") + stmt->getString("last_name");
        if(find(selectedAuthors.begin(),selectedAuthors.end(),authorId) != selectedAuthors.end()){
            gtk_list_store_append(m_selectedAuthorsListStore,&selectedAuthorIter);
            gtk_list_store_set(m_selectedAuthorsListStore,&selectedAuthorIter,
                       0,authorId,
                       1,name.c_str(),-1);
        } else {
            gtk_list_store_append(m_authorsListStore,&authorIter);
            gtk_list_store_set(m_authorsListStore,&authorIter,
                       0,authorId,
                       1,name.c_str(),-1);
        }
    });
}

void SaveBookController::loadPublications(){
    GtkTreeIter publicationIter;
    gtk_list_store_clear(m_publicationListStore);
    int countPublications = 0;
    Publication::all<Publication>([&](Statement* stmt,Bundle* data){
        gtk_list_store_append(m_publicationListStore,&publicationIter);
        gtk_list_store_set(m_publicationListStore,&publicationIter,
                       0,Strings::toString(stmt->getInt("publication_id")).c_str(),
                       1,stmt->getString("publication_name").c_str(),-1);
        countPublications++;
    });

    if(countPublications == 0){
        gtk_widget_set_sensitive(m_editPublicationBtn,false);
        gtk_widget_set_sensitive(m_deletePublicationBtn,false);
    } else {
        gtk_widget_set_sensitive(m_editPublicationBtn,true);
        gtk_widget_set_sensitive(m_deletePublicationBtn,true);
    }

    if(m_book->exists()){
        int publicationId = m_book->fetch()->getInt("publication_id");
        gtk_combo_box_set_active_id(GTK_COMBO_BOX(m_publicationComboBox),Strings::toString(publicationId).c_str());
    } else {
        gtk_combo_box_set_active(GTK_COMBO_BOX(m_publicationComboBox),0);
    }
}