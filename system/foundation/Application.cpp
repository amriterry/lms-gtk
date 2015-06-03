#include "Application.h"

#include <iostream>
#include <string>
#include <functional>
#include <algorithm>
#include <gtk/gtk.h>

#include <system/facades/Facade.h>

#include <system/facades/ConfigManagerFacade.h>

#include <system/config/ConfigRepository.h>
#include <system/foundation/ProviderRepository.h>
#include <system/controller/ControllerRepository.h>
#include <system/config/ConfigManager.h>
#include <system/controller/ControllerLifeCycleManager.h>
#include <system/controller/ControllerManager.h>
#include <system/controller/ControllerStateManager.h>
#include <system/foundation/ServiceProviderManager.h>
#include <system/foundation/PathManager.h>

#include <system/events/EventDispatcher.h>
#include <system/events/EventServiceProvider.h>
#include <system/views/ViewManager.h>
#include <system/views/ViewServiceProvider.h>

#include <vendor/rapidjson/document.h>

using namespace std;
using namespace tuber;

Application* Application::m_instance = nullptr;

/** \brief Application 'Ctor'
 *
 * \param appId string The application id used by GtkApplication ("org.example.web")
 * \param appName string The application name used to display in the title bar of the window if not changed
 * \param flags GApplicationFlags The GApplicationFlags used by GApplication object
 * \return void
 *
 */

Application::Application(string appId,string appName,GApplicationFlags flags) : m_appId(appId), m_appName(appName), m_flags(flags){
    g_message("Application: Application Created");

    this->m_prebooted = false;
    this->m_booted = false;

    Facade::setFacadeApplication(this);

    this->registerBaseManagers();
    this->resolveManagerDependencies();

    this->registerBaseServiceProviders();
    this->resolveServiceDependencies();

    this->registerBaseApplicationEvents();

    this->registerBasePaths();

    this->preboot();
}

/** \brief Application Destructor - Deletes the ServiceProviders contained in the application
 *
 * \return void
 *
 */

Application::~Application(){
    g_message("Application: Application Deleted");
}

/** \brief create - Creates a Application object and stores it in a static variable and returns the object pointer
 *
 * \param appId The application id used by GtkApplication ("org.example.web")
 * \param appName The application name used to display in the title bar of the window if not changed
 * \param flags The GApplicationFlags used by GApplication object
 * \return Application* The pointer to the application object
 *
 */

Application* Application::create(string appId,string appName,GApplicationFlags flags){
    if(m_instance == nullptr){
        m_instance = new Application(appId,appName,flags);
        //  @todo check for Not enough memory
    }
    return getInstance();
}

/** \brief getInstance - Returns the pointer instance of Application object
 *
 * \return Application*
 *
 */

Application* Application::getInstance(){
    return m_instance;
}

/** \brief getAppName - Returns the application name
 *
 * \return string
 *
 */

string Application::getAppName(){
    return this->m_appName;
}

/** \brief getGtkApp - Returns the GtkApplication object
 *
 * \return GtkApplication*
 *
 */

GtkApplication* Application::getGtkApp(){
    return this->m_GtkApp;
}

ConfigManager* Application::getConfigManager(){
    return this->m_configManager;
}

ControllerManager* Application::getControllerManager(){
    return this->m_controllerManager;
}

ServiceProviderManager* Application::getServiceProviderManager(){
    return this->m_serviceProviderManager;
}

/** \brief getEventManager - Returns the Event Manager of the application
 *
 * \return EventDispatcher*
 *
 */

EventDispatcher* Application::getEventDispatcher(){
    return this->m_eventDispatcher;
}

ViewManager* Application::getViewManager(){
    return this->m_viewManager;
}

/** \brief isPrebooted - Checks if the application is 'Prebooted'
 *
 * \return bool If the application is prebooted returns true else false
 *
 */

bool Application::isPrebooted(){
    return this->m_prebooted;
}

/** \brief isBooted - Checks if the application is 'Booted'
 *
 * \return bool If the application is booted returns true else false
 *
 */

bool Application::isBooted(){
    return this->m_booted;
}

void Application::resolveManagerDependencies(){
    this->m_pathManager = this->resolve<PathManager>("PathManager");
    this->m_configManager = this->resolve<ConfigManager>("ConfigManager");
    this->m_controllerManager = this->resolve<ControllerManager>("ControllerManager");
    this->m_serviceProviderManager = this->resolve<ServiceProviderManager>("ServiceProviderManager");
}

void Application::resolveServiceDependencies(){
    this->m_eventDispatcher = this->resolve<EventDispatcher>("EventDispatcher");
    this->m_viewManager = this->resolve<ViewManager>("ViewManager");
}

void Application::registerBasePaths(){
    g_message("Application: setting base application paths");
    this->m_pathManager->setPath("config","config/")
                        ->setPath("views","views/")
                        ->setPath("data","data/");
}

void Application::registerBaseManagers(){
    g_message("Application: Registering Base Managers");
    this->bindShared("ConfigManager",[&](Container* container) -> IBindable*{
        return new ConfigManager(this);
    });

    this->bindShared("ServiceProviderManager",[&](Container* container) -> IBindable*{
        return new ServiceProviderManager(this);
    });

    this->bindShared("ControllerLifeCycleManager",[&](Container* container) -> IBindable*{
        return new ControllerLifeCycleManager(this);
    });

    this->bindShared("ControllerManager",[&](Container* container) -> IBindable*{
        ControllerLifeCycleManager* lifeCycleManager = this->resolve<ControllerLifeCycleManager>("ControllerLifeCycleManager");
        return new ControllerManager(this,lifeCycleManager);
    });

    this->bind("ControllerStateManager",[&](Container* container) -> IBindable*{
        return new ControllerStateManager();
    });

    this->bindShared("PathManager",[&](Container* container) -> IBindable* {
        return new PathManager();
    });
}

void Application::registerBaseServiceProviders(){
    g_message("Application: Registering Base ServiceProviders");
    this->getServiceProviderManager()->registerProvider(new EventServiceProvider(this));
    this->getServiceProviderManager()->registerProvider(new ViewServiceProvider(this));
}

void Application::registerBaseApplicationEvents(){
    g_message("Application: Registering Base Application Events");
    this->m_eventDispatcher->listen("application.preboot",[&](IEventPublisher* publisher){
        onPreboot(publisher);
    },this);

    this->m_eventDispatcher->listen("application.boot",[&](IEventPublisher* publisher){
        onBoot(publisher);
    },this);

    this->m_eventDispatcher->listen("application.quit",[&](IEventPublisher* publisher){
        onQuit(publisher);
    },this);
}

void Application::registerBaseGApplicationSignals(){
    g_message("Application: Registering Base GApplication signals");
    g_signal_connect(m_GtkApp,"activate",G_CALLBACK(static_cast<ApplicationCallback>([](GtkApplication* app,gpointer data) -> void{
        Application* application = static_cast<Application*>(data);
        for(auto it = application->m_activateCallbacks.begin();it != application->m_activateCallbacks.end(); ++it){
            it->first(app,it->second);
        }
    })),this);

    g_signal_connect(m_GtkApp,"shutdown",G_CALLBACK(static_cast<ApplicationCallback>([](GtkApplication* app,gpointer data) -> void{
        Application* application = static_cast<Application*>(data);
        for(auto it = application->m_shutdownCallbacks.begin();it != application->m_shutdownCallbacks.end(); ++it){
            it->first(app,it->second);
        }

        application->quit();
    })),this);
}

void Application::preboot(){
    this->m_prebooted = true;
    this->m_eventDispatcher->fire("application.preboot");
}

void Application::boot(){
    this->m_booted = true;
    this->m_eventDispatcher->fire("application.boot");
}

void Application::quit(){
    Application::getInstance()->m_eventDispatcher->fire("application.quit");
    delete m_instance;
}

void Application::bootUp(){
    this->m_GtkApp = gtk_application_new(this->m_appId.c_str(),this->m_flags);
    g_message("Application: GtkApplication created");

    this->setActivateCallback([&](GtkApplication* gapp,gpointer data){
        g_message("Application: GtkApplication activated");
        Application* app = static_cast<Application*>(data);
        GtkWidget* window = gtk_application_window_new(gapp);
        gtk_window_set_title(GTK_WINDOW(window),app->getAppName().c_str());
        guint window_id = gtk_application_window_get_id(GTK_APPLICATION_WINDOW(window));
        g_message("Default Stage Id: %d",window_id);
        app->getViewManager()->setDefaultStage(window_id);
    },this);

    //getViewManager()->setDefaultStage(0);

    this->registerBaseGApplicationSignals();
    this->boot();
}

int Application::run(int &argc,char** &argv){
    if(!isBooted()){
        this->bootUp();
    }

    loadDefaultController();

    int status = g_application_run(G_APPLICATION(this->m_GtkApp),argc,argv);
    return status;
}

void Application::loadDefaultController(){
    g_message("Application: Setting Up Default Controller");

    //  Get default Controller from json config file
    //  Create Request object
    //  Send Request object to ControllerManager to load controller

    this->setActivateCallback([](GtkApplication* app,gpointer data){
        g_message("Trying to load Controller after activated");
        ControllerManager* manager = (ControllerManager*) (data);
        manager->loadController(ConfigManagerFacade::get()->getDefaultController());
        g_message("End of callback");
    },this->m_controllerManager);

    g_message("Application: loadDefaultController() closing");
}

void Application::onPreboot(IEventPublisher* publisher){
    g_message("Application: Prebooted");
    for(auto it: this->m_prebootCallbacks){
        it(static_cast<Application*>(publisher));
    }
}

void Application::onBoot(IEventPublisher* publisher){
    g_message("Application: Booted");
    for(auto it: this->m_bootCallbacks){
        it(static_cast<Application*>(publisher));
    }
}

void Application::onQuit(IEventPublisher* publisher){
    g_message("Application: Quiting");
    for(auto it: this->m_quitCallbacks){
        it(static_cast<Application*>(publisher));
    }
}

void Application::setPrebootCallback(ApplicationEventCallback prebootCallback){
    this->m_prebootCallbacks.push_back(prebootCallback);
}
void Application::setBootCallback(ApplicationEventCallback bootCallback){
    this->m_bootCallbacks.push_back(bootCallback);
}

void Application::setQuitCallback(ApplicationEventCallback quitCallback){
    this->m_quitCallbacks.push_back(quitCallback);
}

void Application::setActivateCallback(ApplicationCallback activatedCallback,gpointer data){
    this->m_activateCallbacks.insert({activatedCallback,data});
}

void Application::setShutdownCallback(ApplicationCallback shutdownCallback,gpointer data){
    this->m_shutdownCallbacks.insert({shutdownCallback,data});
}

void Application::loadConfiguration(ConfigRepository* configRepo){
    g_message("Application: Loading loadConfiguration");
    rapidjson::Document *configDocument = configRepo->getConfigDocument();
    this->m_configManager->loadConfigDocument(configDocument);
}

void Application::loadProviders(ProviderRepository* providerRepo){
    this->m_serviceProviderManager->loadProviders(providerRepo);
}

void Application::loadControllers(ControllerRepository* controllerRepo){
    this->m_controllerManager->loadControllers(controllerRepo);
}
