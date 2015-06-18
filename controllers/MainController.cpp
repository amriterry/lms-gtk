#include <gtk/gtk.h>

#include "MainController.h"

#include <system/facades/Facades.h>

using namespace std;
using namespace tuber;

void MainController::onCreate(Bundle* package){
	Controller::onCreate(package);
    g_message("MainController: onCreate()");
    setContentView("views/main.glade");
}

void MainController::onPause(){
    g_message("MainController: Pausing");
    finish();
}
