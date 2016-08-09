#include <gtk/gtk.h>
#include <system/views/ViewManager.h>
#include <system/foundation/Application.h>
#include <system/controller/Controller.h>

namespace tuber{

/**
 * @brief ViewManager 'ctor'
 *
 * @param app Injecting Application to Manager
 */
ViewManager::ViewManager(Application* app): m_app(app){
	this->m_gtkApp = app->getGtkApp();
}

/**
 * @brief Sets the default stageId
 *
 * @param stageId StageId to be set as default
 */
void ViewManager::setDefaultStage(int stageId){
	this->m_defaultStageId = stageId;
}

/**
 * @brief Getter for the default Stage
 * @return Returns the window by the default stage Id
 */
GtkWindow* ViewManager::getDefaultStage(){
	return gtk_application_get_window_by_id(this->m_app->getGtkApp(),this->m_defaultStageId);
}

/**
 * @brief Sets a scene into the default stage
 *
 * @param controller Controller on which the builder is to be bound
 * @param viewPath Path of the view
 * @param rootName Root name of the view
 * @param topLevel Is the root top level?
 * @return Returns the root GtkWidget created
 */
GtkWidget* ViewManager::setScene(Controller* controller,string viewPath,string rootName,bool topLevel){
    //  Adding absolute path to the view path
    viewPath = m_app->resolve<PathManager>("PathManager")->getPath("views") + viewPath;
    //  Creating and adding new builder
    GtkBuilder* builder = gtk_builder_new();
    gtk_builder_add_from_file(builder,viewPath.c_str(),NULL);

    //  Binding Builder to the controller so that we can get GtkWidgets from the controller directly
    controller->bindGtkBuilder(builder);

    //  Setting scene into the stage
    return setScene(GTK_WIDGET(gtk_builder_get_object(builder,rootName.c_str())),topLevel);
}

/**
 * @brief Sets the scene to a default stage
 *
 * @param scene Scene to load into the stage
 * @param topLevel Is the root top level?
 *
 * @return Returns the root GtkWidget Created
 */
GtkWidget* ViewManager::setScene(GtkWidget* scene,bool topLevel){
    return setScene(getDefaultStage(),scene,topLevel);
}

/**
 * @brief Sets scene into a given stage
 *
 * @param stage Stage in which the scene is to be loaded
 * @param scene Scene to load
 * @param topLevel Is the root top level?
 * @return Returns scene root
 *
 * @todo Change stage to GtkWidget instead of GtkWindow
 */
GtkWidget* ViewManager::setScene(GtkWindow* stage,GtkWidget* scene,bool topLevel){
    if(!topLevel){
        //  If the root is not toplevel
        //  add the scene to the stage
        gtk_container_add(GTK_CONTAINER(stage),scene);
        gtk_widget_show_all(GTK_WIDGET(stage));
    } else {
        //  If the root is toplevel
        //  Do not add scene to the stage
        //gtk_widget_hide(GTK_WIDGET(stage));
        //gtk_widget_show_all(scene);
    }
    return scene;
}

void ViewManager::setGlobalStylesheet(string filePath){
    GdkDisplay* display = gdk_display_get_default();
    GdkScreen* screen = gdk_display_get_default_screen(display);
    GtkCssProvider* provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider,filePath.c_str(),NULL);
    gtk_style_context_add_provider_for_screen(screen,GTK_STYLE_PROVIDER(provider),GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

}
