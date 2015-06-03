#include <windows.h>

#include <gtk/gtk.h>

#include "SplashController.h"

#include <system/facades/AppFacade.h>
#include <system/facades/ViewFacade.h>
#include <system/facades/RequestFacade.h>
#include <system/facades/ControllerManagerFacade.h>

#include <thread>

using namespace std;
using namespace tuber;

void SplashController::onCreate(Bundle* package){
	g_message("SplashController: onCreate");

    GtkWindow* stage = ViewFacade::get()->getDefaultStage();
	gtk_window_set_decorated(stage,false);

	setContentView("views/splash.glade");
}

void SplashController::onStart(){
    Request* request = RequestFacade::get();
    request->setActionKey("MainController");
    //request->carryPackage(nullptr);
    //Sleep(5000);
    startController(request);
}

void SplashController::onPause(){
    finish();
}
