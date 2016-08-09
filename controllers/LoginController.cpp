#include <gtk/gtk.h>

#include "LoginController.h"

#include <system/facades/Facades.h>
#include <system/database/model/Model.h>

using namespace std;
using namespace tuber;

void LoginController::onCreate(Bundle* package){
	Controller::onCreate(package);

	GtkWindow* stage = View::get()->getDefaultStage();

	setContentView("login.glade");

	GtkWidget* loginBtn = getSceneObj("loginBtn");
	GtkWidget* usernameEntry = getSceneObj("usernameEntry");
	GtkWidget* passwordEntry = getSceneObj("passwordEntry");
	GtkWidget* errorDialog = getSceneObj("loginErrorDialog");

	Bundle* entries = BundleFacade::get();
	entries->putExtra("usernameEntry",usernameEntry)
			->putExtra("passwordEntry",passwordEntry)
			->putExtra("controller",this)
			->putExtra("loginErrorDialog",errorDialog);

	GAppEventCallback callback = [](GtkWidget* widget,gpointer data){
			Bundle* entries = static_cast<Bundle*>(data);

			GtkWidget* usernameEntry = entries->getGeneric<GtkWidget>("usernameEntry");
			GtkWidget* passwordEntry = entries->getGeneric<GtkWidget>("passwordEntry");

			string username = gtk_entry_get_text(GTK_ENTRY(usernameEntry));
			string password = gtk_entry_get_text(GTK_ENTRY(passwordEntry));

			QueryRow user = DB::table("users")
								->select({"user_id"})
								->where("users.username","=",username)
								->where("users.password","=",Hash::make(password))
								->first();

            if(Auth::check(username,password)){
                g_message("Logged in");

                Request* request = RequestFacade::get();
                request->setActionKey("MainController");
                ControllerManagerFacade::get()->loadController(request,entries->getGeneric<LoginController>("controller"));
            } else {
                gtk_entry_set_text(GTK_ENTRY(passwordEntry),"");
                GtkWidget* dialog = entries->getGeneric<GtkWidget>("loginErrorDialog");
                gtk_dialog_run(GTK_DIALOG(dialog));
                gtk_widget_hide(dialog);
				g_message("Not Logged in");
            }

			//g_message("Username: %s and Password: %s and User Id: %i",username.c_str(),Hash::get()->encrypt(password).c_str(),Auth::id());
	};

    g_signal_connect(passwordEntry,"activate",G_CALLBACK(callback),entries);
	g_signal_connect(loginBtn,"clicked",G_CALLBACK(callback),entries);
}

void LoginController::onPause(){
    Controller::onPause();
    finish();
}
