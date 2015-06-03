#include <gtk/gtk.h>
#include <system/views/ViewManager.h>
#include <system/foundation/Application.h>
#include <system/controller/Controller.h>

namespace tuber{

ViewManager::ViewManager(Application* app): m_app(app){
	g_message("ViewManager: created ViewManager object");
	this->m_gtkApp = app->getGtkApp();
}

void ViewManager::setDefaultStage(int stageId){
	this->m_defaultStageId = stageId;
}

GtkWindow* ViewManager::getDefaultStage(){
	return gtk_application_get_window_by_id(this->m_app->getGtkApp(),this->m_defaultStageId);
}

GtkWidget* ViewManager::setScene(Controller* controller,string viewPath,string rootName,bool topLevel){
	g_message("ViewManager: Loading View from File: %s",viewPath.c_str());
    GtkBuilder* builder = gtk_builder_new();
    gtk_builder_add_from_file(builder,viewPath.c_str(),NULL);
    controller->bindGtkBuilder(builder);
    return setScene(GTK_WIDGET(gtk_builder_get_object(builder,rootName.c_str())),topLevel);
}

GtkWidget* ViewManager::setScene(GtkWidget* scene,bool topLevel){
    return setScene(getDefaultStage(),scene,topLevel);
}

GtkWidget* ViewManager::setScene(GtkWindow* stage,GtkWidget* scene,bool topLevel){
    if(!topLevel){
        gtk_container_add(GTK_CONTAINER(stage),scene);
        gtk_widget_show_all(GTK_WIDGET(stage));
    } else {
        gtk_widget_hide(GTK_WIDGET(stage));
        gtk_widget_show_all(scene);
    }
    return scene;
}

}
