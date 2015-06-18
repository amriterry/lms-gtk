#include <gtk/gtk.h>

#include "LoginController.h"

#include <system/facades/Facades.h>
#include <system/database/model/Model.h>

using namespace std;
using namespace tuber;

void LoginController::onCreate(Bundle* package){
	Controller::onCreate(package);

	g_message("LoginController: onCreate()");

	GtkWindow* stage = View::get()->getDefaultStage();
	gtk_window_set_decorated(stage,true);
	setContentView("views/login.glade");

	//Get from builder
	GtkWidget* loginBtn = getSceneObj("loginBtn");
	GtkWidget* usernameEntry = getSceneObj("usernameEntry");
	GtkWidget* passwordEntry = getSceneObj("passwordEntry");

	Bundle* entries = BundleFacade::get();
	entries->putExtra("usernameEntry",usernameEntry)
			->putExtra("passwordEntry",passwordEntry)
			->putExtra("controller",this);

	GAppEventCallback callback = [](GtkWidget* widget,gpointer data){
			Bundle* entries = static_cast<Bundle*>(data);

			GtkWidget* usernameEntry = entries->getGeneric<GtkWidget>("usernameEntry");
			GtkWidget* passwordEntry = entries->getGeneric<GtkWidget>("passwordEntry");

			string username = gtk_entry_get_text(GTK_ENTRY(usernameEntry));
			string password = gtk_entry_get_text(GTK_ENTRY(passwordEntry));

			if(username == "admin" && password == "admin"){
					g_message("Logged in");

					Request* request = RequestFacade::get();
					request->setActionKey("MainController");
					ControllerManagerFacade::get()->loadController(request,entries->getGeneric<LoginController>("controller"));
			} else {
					g_message("Not Logged in");
			}

			g_message("Username: %s and Password: %s",username.c_str(),Hash::get()->encrypt(password).c_str());
	};

	g_signal_connect(loginBtn,"clicked",G_CALLBACK(callback),entries);

}

void LoginController::onStart(){
}

void LoginController::onPause(){
    g_message("LoginController: Pausing");
    finish();
}
