#include <gtk/gtk.h>

#include "MainController.h"

#include <system/facades/Facades.h>

using namespace std;
using namespace tuber;

MainController::MainController(Application *app,ControllerStateManager* stateManager,ViewManager* viewManager) : Controller(app,stateManager,viewManager){
    g_message("MainController: Main Controller Created");
}

void MainController::onCreate(Bundle* package){
	Controller::onCreate(package);
    g_message("MainController: onCreate()");

}

void MainController::onPause(){
    g_message("MainController: Pausing");
    finish();
}
