#include <windows.h>

#include <gtk/gtk.h>

#include "SplashController.h"

#include <system/facades/App.h>
#include <system/facades/View.h>
#include <system/facades/RequestFacade.h>
#include <system/facades/ControllerManagerFacade.h>

#include <thread>

using namespace std;
using namespace tuber;

void SplashController::onCreate(Bundle* package){
	g_message("SplashController: onCreate");
	GdkDisplay *display;
    GdkScreen *screen;
    GtkCssProvider *provider;

	setContentView("views/splash.glade",true);

    GtkWidget* root = getSceneRoot();
	gtk_window_set_decorated(GTK_WINDOW(root),false);

    GAppEventCallback closeCallback = [&](GtkWidget* widget,gpointer data){
        g_application_quit(G_APPLICATION(static_cast<GtkApplication*>(data)));
    };
	GtkWidget* closeBtn = getSceneObj("closeBtn");
    g_signal_connect(closeBtn,"clicked",G_CALLBACK(closeCallback),getGtkApplication());

    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);

    provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider,"splash.css",NULL);
    gtk_style_context_add_provider_for_screen(screen,GTK_STYLE_PROVIDER(provider),GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

void SplashController::onStart(){
    GSourceFunc func = [&](gpointer data) -> gboolean{
        Controller* controller = (Controller*) (data);
        Request* request = RequestFacade::get();
        request->setActionKey("LoginController");
        controller->startController(request);
        return false;
    };
    g_timeout_add(5000,func,this);

}

void SplashController::onPause(){
    finish();
}
