#include <windows.h>
#include <cstdlib>

#include "SplashController.h"

#include <models/User.h>
#include <models/Book.h>
#include <models/Role.h>
#include <models/AclAction.h>
#include <models/AclEntity.h>
#include <models/AclPrivilege.h>
#include <models/AclRolePrivilege.h>

using namespace std;
using namespace tuber;

void SplashController::onCreate(Bundle* package){
	g_message("SplashController: onCreate");

	setContentView("splash.glade",true);

    GtkWidget* root = getSceneRoot();
    gtk_widget_show_all(root);
    gtk_widget_set_size_request(root,511,300);
    gtk_window_set_resizable(GTK_WINDOW(root),false);
	gtk_window_set_decorated(GTK_WINDOW(root),false);

    GAppEventCallback closeCallback = [&](GtkWidget* widget,gpointer data){
        g_application_quit(G_APPLICATION(static_cast<GtkApplication*>(data)));
    };
	GtkWidget* closeBtn = getSceneObj("closeBtn");
	if(gtk_widget_get_visible(this->getSceneObj("progressBar"))){
        g_message("***************Progress Bar visible");
	} else {
        g_message("***************Progress Bar Not visible");
	}
	//gtk_widget_set_can_focus(closeBtn,false);
    g_signal_connect(closeBtn,"clicked",G_CALLBACK(closeCallback),getGtkApplication());
}

void SplashController::onStart(){
    GSourceFunc progressFunc = [&](gpointer data) -> gboolean{
        static double fraction = 0.0;
        Controller* controller = (Controller*) (data);

        GtkWidget* bar = controller->getSceneObj("progressBar");
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(bar),fraction/100.0);

        fraction += 1;
        if(fraction > 100.0){
            Request* request = RequestFacade::get();
            request->setActionKey("LoginController");
            controller->startController(request);
            return false;
        } else {
            return true;
        }
    };
    g_timeout_add(20,progressFunc,this);

    clock_t begin = clock();

    //  Initializing tables and data
    vector<string> tables;
    DB::rawSelect("SELECT name FROM sqlite_master WHERE type = \"table\"",[&](Statement* stmt,Bundle* data){
        tables.push_back(stmt->getString("name"));
    });

    if(!Stl::hasValue("users",tables)){
        DB::raw("CREATE TABLE IF NOT EXISTS users(\
            user_id INTEGER PRIMARY KEY AUTOINCREMENT,\
            first_name TEXT NOT NULL,\
            middle_name TEXT,\
            last_name TEXT NOT NULL,\
            username TEXT NOT NULL,\
            email TEXT,\
            password TEXT NOT NULL,\
            valid_upto REAL,\
            role_id INTEGER NOT NULL,\
            status_id INTEGER NOT NULL,\
            created_at REAL,\
            updated_at REAL)");

        User* user = new User();
        user->getDirty()
            ->putExtra("first_name","Admin")
            ->putExtra("last_name","Admin")
            ->putExtra("username","admin")
            ->putExtra("email","admin@example.com")
            ->putExtra("password",Hash::make("admin"))
            ->putExtra("role_id",1)
            ->putExtra("status_id",1);

        if(user->saveRow()){
            g_message("SplashController: Default user saved");
        } else {
            g_message("SplashController: Default user could not be saved");
        }
        delete user;
    }

    if(!Stl::hasValue("books",tables)){
        DB::raw("CREATE TABLE IF NOT EXISTS books(\
            book_id INTEGER PRIMARY KEY AUTOINCREMENT,\
            book_name TEXT NOT NULL,\
            isbn TEXT NOT NULL,\
            edition INTEGER,\
            publication_id INTEGER NOT NULL,\
            created_at REAL,\
            updated_at REAL,\
            status_id INTEGER NOT NULL)");
    }

    if(!Stl::hasValue("book_copies",tables)){
        DB::raw("CREATE TABLE IF NOT EXISTS book_copies(\
                copy_id INTEGER PRIMARY KEY AUTOINCREMENT,\
                book_id INTEGER NOT NULL,\
                copy_code TEXT NOT NULL,\
                status_id INTEGER NOT NULL)");
    }

    if(!Stl::hasValue("publications",tables)){
        DB::raw("CREATE TABLE IF NOT EXISTS publications(\
                publication_id INTEGER PRIMARY KEY AUTOINCREMENT,\
                publication_name TEXT NOT NULL)");
    }

    if(!Stl::hasValue("book_authors",tables)){
        DB::raw("CREATE TABLE IF NOT EXISTS book_authors(\
                reln_id INTEGER PRIMARY KEY AUTOINCREMENT,\
                book_id INTEGER NOT NULL,\
                author_id INTEGER NOT NULL)");
    }

    if(!Stl::hasValue("authors",tables)){
        DB::raw("CREATE TABLE IF NOT EXISTS authors(\
                author_id INTEGER PRIMARY KEY AUTOINCREMENT,\
                first_name TEXT NOT NULL,\
                middle_name TEXT NOT NULL,\
                last_name INTEGER NOT NULL,\
                created_at REAL,\
                updated_at REAL)");
    }

    if(!Stl::hasValue("issues",tables)){
        DB::raw("CREATE TABLE IF NOT EXISTS issues(\
                issue_id INTEGER PRIMARY KEY AUTOINCREMENT,\
                copy_id INTEGER NOT NULL,\
                user_id INTEGER NOT NULL,\
                last_issue_id INTEGER,\
                issued_at REAL NOT NULL,\
                deadline_at REAL NOT NULL,\
                status_id INTEGER NOT NULL)");
    }

    if(!Stl::hasValue("roles",tables)){
        DB::raw("CREATE TABLE IF NOT EXISTS roles(\
                role_id INTEGER PRIMARY KEY AUTOINCREMENT,\
                role_name TEXT NOT NULL)");

        Role* role = new Role();
        role->getDirty()
            ->putExtra("role_name","Administrator");

        role->saveRow();

        role->getDirty()
            ->putExtra("role_name","Librarian");

        role->saveRow();

        role->getDirty()
            ->putExtra("role_name","Student");

        role->saveRow();
    }

    if(!Stl::hasValue("aclrole_privileges",tables)){
        DB::raw("CREATE TABLE IF NOT EXISTS aclrole_privileges(\
                reln_id INTEGER PRIMARY KEY AUTOINCREMENT,\
                role_id INTEGER NOT NULL,\
                privilege_id INTEGER NOT NULL)");

        AclRolePrivilege* rolePrivilege = new AclRolePrivilege();

        for(int i = 1;i <= 3;i++){
            if(i == 1){
                for(int j = 9;j <= 12;j++){
                    rolePrivilege->getDirty()->putExtra("role_id",i)->putExtra("privilege_id",j);
                    rolePrivilege->saveRow(); 
                } 
            } else  if(i == 2){
                for(int j = 1;j <= 24;j++){
                    if(j == 9 || j == 11 || j == 12){
                        continue;
                    }
                    rolePrivilege->getDirty()->putExtra("role_id",i)->putExtra("privilege_id",j);
                    rolePrivilege->saveRow();
                }
            } else {
                rolePrivilege->getDirty()->putExtra("role_id",i)->putExtra("privilege_id",2);
                rolePrivilege->saveRow();

                rolePrivilege->getDirty()->putExtra("role_id",i)->putExtra("privilege_id",22);
                rolePrivilege->saveRow();
            }
        }
        delete rolePrivilege;
    }

    if(!Stl::hasValue("aclprivileges",tables)){
        DB::raw("CREATE TABLE IF NOT EXISTS aclprivileges(\
                privilege_id INTEGER PRIMARY KEY AUTOINCREMENT,\
                entity_id INTEGER NOT NULL,\
                action_id INTEGER NOT NULL)");

        AclPrivilege* privileges = new AclPrivilege();

        for(int i = 1;i <= 6;i++){
            for(int j = 1;j <= 4;j++){
                privileges->getDirty()
                            ->putExtra("entity_id",i)
                            ->putExtra("action_id",j);
                privileges->saveRow();
            }
        }

        delete privileges;
    }

    if(!Stl::hasValue("aclentities",tables)){
        DB::raw("CREATE TABLE IF NOT EXISTS aclentities(\
                entity_id INTEGER PRIMARY KEY AUTOINCREMENT,\
                entity_name TEXT NOT NULL)");

        AclEntity* entities = new AclEntity();

        entities->getDirty()->putExtra("entity_name","Book");
        entities->saveRow();

        entities->getDirty()->putExtra("entity_name","Issue");
        entities->saveRow();

        entities->getDirty()->putExtra("entity_name","User");
        entities->saveRow();

        entities->getDirty()->putExtra("entity_name","Author");
        entities->saveRow();

        entities->getDirty()->putExtra("entity_name","Publication");
        entities->saveRow();

        entities->getDirty()->putExtra("entity_name","BookCopy");
        entities->saveRow();

        delete entities;
    }

    if(!Stl::hasValue("aclactions",tables)){
        DB::raw("CREATE TABLE IF NOT EXISTS aclactions(\
                action_id INTEGER PRIMARY KEY AUTOINCREMENT,\
                action_name TEXT NOT NULL)");

        AclAction* actions = new AclAction();

        actions->getDirty()->putExtra("action_name","Create");
        actions->saveRow();

        actions->getDirty()->putExtra("action_name","Read");
        actions->saveRow();

        actions->getDirty()->putExtra("action_name","Update");
        actions->saveRow();

        actions->getDirty()->putExtra("action_name","Delete");
        actions->saveRow();

        delete actions;
    }

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    g_message("SplashController: onStart completed at: %f",elapsed_secs);
}

void SplashController::onPause(){
    finish();
}
