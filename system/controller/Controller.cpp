#include <gtk/gtk.h>

#include <system/controller/Controller.h>

#include <system/views/ViewManager.h>
#include <system/foundation/Application.h>
#include <system/controller/ControllerState.h>
#include <system/controller/ControllerManager.h>
#include <system/controller/ControllerStateManager.h>
#include <system/foundation/RunTimeException.h>

namespace tuber{

/**
 * @brief Controller 'ctor'
 * @details Bootstraps the controller
 */
Controller::Controller() {
	this->m_builderBinded = false;
	this->m_rootSceneBinded = false;
	this->m_childController = nullptr;
	this->m_parentController = nullptr;
}

/**
 * @brief Setter for controller dependencies
 * 
 * @param app Application object
 * @param stateManager [description]
 * @param viewManager [description]
 */
void Controller::setControllerDependencies(Application *app,ControllerStateManager* stateManager,ViewManager* viewManager){
    this->m_app = app;
    this->m_stateManager = stateManager;
    this->m_viewManager = viewManager;
}

void Controller::finish(bool bindChild){
	g_message("Controller: called finish() on [%s]",this->getBindingKey().c_str());
	if(!bindChild){
		unrefChildController();
	}
	if(this->m_childController != nullptr){
        //g_message("Controller: deleting child controller [%s] from [%s]",this->m_childController->getBindingKey().c_str(),this->getBindingKey().c_str());
		this->m_app->getControllerManager()->destroyController(this->m_childController);
	}
	this->m_app->getControllerManager()->destroyController(this);
}

void Controller::finish(Bundle* package,bool bindChild){
	//g_message("Controller: called finish() on [%s]",this->getBindingKey().c_str());
	if(!bindChild){
		g_message("Controller: unrefrencing child controller");
		unrefChildController();
	}
	if(this->m_childController != nullptr){
        g_message("Controller: deleting child controller [%s] from [%s]",this->m_childController->getBindingKey().c_str(),this->getBindingKey().c_str());
		this->m_app->getControllerManager()->destroyController(this->m_childController);
	}
	this->m_app->getControllerManager()->destroyController(this,package);
}

void Controller::bindGtkBuilder(GtkBuilder* builder){
	this->m_builder = builder;
	this->m_builderBinded = true;
}

GtkBuilder* Controller::getGtkBuilder(){
	if(this->m_builderBinded){
		return this->m_builder;
	} else {
		throw RunTimeException("GtkBuilder is not binded yet. Load view to bind the builder!");
	}
}

void Controller::destroySceneRoot(){
    if(this->m_rootSceneBinded){
		//g_message("Controller: Destroying scene root widget");
        gtk_widget_destroy(this->m_sceneRoot);
	}
}

void Controller::bindSceneRoot(GtkWidget* widget){
	this->m_sceneRoot = widget;
	this->m_rootSceneBinded = true;
}

Controller* Controller::getChildController(){
	return this->m_childController;
}

Controller* Controller::getParentController(){
	return this->m_parentController;
}

Application* Controller::getApplication(){
	return this->m_app;
}

ViewManager* Controller::getViewManager(){
	return this->m_viewManager;
}

GtkWidget* Controller::getSceneRoot(){
	return this->m_sceneRoot;
}

GtkWidget* Controller::getSceneObj(string objName){
    return GTK_WIDGET(gtk_builder_get_object(this->m_builder,objName.c_str()));
}

GObject* Controller::getSceneRawObj(string objName){
    return gtk_builder_get_object(this->m_builder,objName.c_str());
}

/**
 * @brief Getter for GtkApplication
 * @return GtkApplication created during boot process
 */
GtkApplication* Controller::getGtkApplication(){
    return this->m_app->getGtkApp();
}

ControllerStateManager* Controller::getStateManager(){
	return this->m_stateManager;
}

void Controller::unrefChildController(){
	if(this->m_childController != nullptr){
		this->m_childController->unrefParentController();
		this->m_childController = nullptr;
	}
}

void Controller::unrefParentController(){
	if(this->m_parentController != nullptr){
		this->m_parentController = nullptr;
	}
}

void Controller::startController(Request* controllerRequest){
	this->m_app->getControllerManager()->loadController(controllerRequest,this);
}

void Controller::startController(string controllerKey,Bundle* package){
	Request* controllerRequest = RequestFacade::get();
    controllerRequest->setActionKey(controllerKey);
    if(package != nullptr){
    	controllerRequest->carryPackage(package);
    }
    this->startController(controllerRequest);
}

void Controller::setChildController(Controller* controller){
	//g_message("Controller: Setting child controller [%s] for [%s]",controller->getBindingKey().c_str(),this->getBindingKey().c_str());
	this->m_childController = controller;
}

void Controller::setParentController(Controller* controller){
	//g_message("Controller: Setting parent controller [%s] for [%s]",controller->getBindingKey().c_str(),this->getBindingKey().c_str());
	this->m_parentController = controller;
}

void Controller::setTopLevelView(string viewPath){
	this->setContentView(viewPath,true);
}

void Controller::setContentView(string viewPath,bool topLevel){
    this->setContentView(viewPath,"root",topLevel);
}

void Controller::setContentView(string viewPath,string nodeName,bool toplevel){
    bindSceneRoot(this->getViewManager()->setScene(this,viewPath,nodeName,toplevel));
}

list<string> Controller::getHierarchy(){
	Controller* parent = m_parentController;
    Controller* child = m_childController;
    list<string> hierarchy;

    while(parent != nullptr){
        hierarchy.push_front(parent->getBindingKey());
        parent = parent->getParentController();
   	}

    hierarchy.push_back(this->getBindingKey());

    while(child != nullptr){
        hierarchy.push_back(child->getBindingKey());
        child = child->getParentController();
    }

    return hierarchy;
}

Controller::~Controller(){
	this->m_app->destroyBindable(this->m_stateManager);
	if(this->m_builderBinded){
		g_object_unref(G_OBJECT(this->m_builder));
	}
	//g_message("Controller: Trying to delete controller scene root");
	this->destroySceneRoot();
	g_message("%s: Destructor",getBindingKey().c_str());
}

}
