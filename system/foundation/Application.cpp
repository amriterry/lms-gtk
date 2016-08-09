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

namespace tuber{

/**
 * Application object's pointer to store the actual application
 */
Application* Application::m_instance = nullptr;

/**
 * @brief Application 'ctor'
 * @details Does all application preboot process
 *
 * @param appId Application Identifier
 * @param appName Application Name
 * @param flags GApplicationFlags to create GApplicaton object
 */
Application::Application(string appId,string appName,GApplicationFlags flags) : m_appId(appId), m_appName(appName), m_flags(flags){
    g_message("Application: Application Created");

    //  Set both prebooted and booted state variables to false as it is the
    //  initial condition at first
    this->m_prebooted = false;
    this->m_booted = false;

    //  Setting Application object into the Facade class so that Facades can
    //  resolve objects out of container
    Facade::setFacadeApplication(this);

    //  Registering Base Managers so that stuffs like event handling can be done
    this->registerBaseManagers();
    //  Resolving Manager dependencies and injecting into this object so that
    //  application can use manager objects
    this->resolveManagerDependencies();

    //  Registering Base Serviceproviders so that application can boot using the
    //  corresponding services
    this->registerBaseServiceProviders();
    //  Resolving services from the container and injecting into this object so that
    //  Application can use the services
    this->resolveServiceDependencies();

    //  Registering Base application events such as 'boot' event so that
    //  event handlers can handle events properly
    this->registerBaseApplicationEvents();

    //  Registering Base paths in the path manager so that config and data can be loaded later
    this->registerBasePaths();

    //  Prebooting the application
    this->preboot();
}

/**
 * @brief Application Destructor
 */
Application::~Application(){
    g_message("Application: Application Deleted");
}

/**
 * @brief Creates the Application object
 *
 * @param appId Application Identifier
 * @param appName Application Name
 * @param flags GApplicationFlags to created GApplication object
 * @return Application*
 */
Application* Application::create(string appId,string appName,GApplicationFlags flags){
    if(m_instance == nullptr){
        m_instance = new Application(appId,appName,flags);
        //  @todo check for Not enough memory
    }
    return getInstance();
}

/**
 * @brief Returns the application object
 * @return Application*
 */
Application* Application::getInstance(){
    return m_instance;
}

/**
 * @brief Returns the application name
 * @return string
 */
string Application::getAppName(){
    return this->m_appName;
}

/**
 * @brief Returns the GtkApplication object of the application
 * @return GtkApplication*
 */
GtkApplication* Application::getGtkApp(){
    return this->m_GtkApp;
}

/**
 * @brief Returns the View Manager object
 * @return ViewManager*
 */
ViewManager* Application::getViewManager(){
    return this->m_viewManager;
}

/**
 * @brief Returns the Path Manager object
 * @return PathManager*
 */
PathManager* Application::getPathManager(){
    return this->m_pathManager;
}

/**
 * @brief Returns the Config Manager object
 * @return ConfigManager*
 */
ConfigManager* Application::getConfigManager(){
    return this->m_configManager;
}

/**
 * @brief Returns the Event Dispatcher object
 * @return EventDispatcher*
 */
EventDispatcher* Application::getEventDispatcher(){
    return this->m_eventDispatcher;
}

/**
 * @brief Returns the Controller Manager object
 * @return ControllerManager*
 */
ControllerManager* Application::getControllerManager(){
    return this->m_controllerManager;
}

/**
 * @brief Returns Service provider manager
 * @return ServiceProviderManager*
 */
ServiceProviderManager* Application::getServiceProviderManager(){
    return this->m_serviceProviderManager;
}

/**
 * @brief Checks if the application is prebooted
 * @return bool
 */
bool Application::isPrebooted(){
    return this->m_prebooted;
}

/**
 * @brief Checks if the application is booted
 * @return bool
 */
bool Application::isBooted(){
    return this->m_booted;
}

/**
 * @brief Resolves and stores the Manager dependencies out of container into the application
 */
void Application::resolveManagerDependencies(){
    //  Resolving PathManager object out of container
    this->m_pathManager = this->resolve<PathManager>("PathManager");
    //  Resolving ConfigManager object out of container
    this->m_configManager = this->resolve<ConfigManager>("ConfigManager");
    //  Resolving ControllerManager object out of container
    this->m_controllerManager = this->resolve<ControllerManager>("ControllerManager");
    //  Resolving ServiceProviderManger object out of container
    this->m_serviceProviderManager = this->resolve<ServiceProviderManager>("ServiceProviderManager");
}

/**
 * @brief Resolves and stores services out of the container into the application
 */
void Application::resolveServiceDependencies(){
    //  Resolving EventDispatcher object out of container
    this->m_eventDispatcher = this->resolve<EventDispatcher>("EventDispatcher");
    //  Resolving ViewManager object out of container
    this->m_viewManager = this->resolve<ViewManager>("ViewManager");
}

/**
 * @brief Registers the base application paths into the path manager
 */
void Application::registerBasePaths(){
    this->m_pathManager->setPath("config","config/")
                        ->setPath("views","views/")
                        ->setPath("data","data/");
}

/**
 * @brief Registers the base manager classes into the container
 */
void Application::registerBaseManagers(){
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

/**
 * @brief Registers Base ServiceProviders into the ServiceProviderManager object
 */
void Application::registerBaseServiceProviders(){
    this->getServiceProviderManager()->registerProvider(new EventServiceProvider(this));
    this->getServiceProviderManager()->registerProvider(new ViewServiceProvider(this));
}

/**
 * @brief Registers Base Application Events
 */
void Application::registerBaseApplicationEvents(){
    //  Registering 'preboot' application events
    this->m_eventDispatcher->listen("application.preboot",[&](IEventPublisher* publisher){
        onPreboot(publisher);
    },this);

    //  Registering 'boot' application events
    this->m_eventDispatcher->listen("application.boot",[&](IEventPublisher* publisher){
        onBoot(publisher);
    },this);

    //  Registering 'quit' application events
    this->m_eventDispatcher->listen("application.quit",[&](IEventPublisher* publisher){
        onQuit(publisher);
    },this);
}

/**
 * @brief Registers Base GApplicationSignals into the GApplication signal system
 */
void Application::registerBaseGApplicationSignals(){
    //  Registering GApplication 'activate' callback
    //  Since only a single signal can be added to the GApplication for 'activate' callback
    //  We emulate multiple 'activate' callbacks by calling all the callbacks inside a single main callback
    g_signal_connect(m_GtkApp,"activate",G_CALLBACK(static_cast<ApplicationCallback>([](GtkApplication* app,gpointer data) -> void{
        Application* application = static_cast<Application*>(data);
        //  Calling each application activate callback from the map
        for(auto it = application->m_activateCallbacks.begin();it != application->m_activateCallbacks.end(); ++it){
            it->first(app,it->second);
        }
    })),this);

    //  Registering GApplication 'shutdown' callback
    //  Similar to activate callback we emulate multiple 'shutdown' callbacks
    g_signal_connect(m_GtkApp,"shutdown",G_CALLBACK(static_cast<ApplicationCallback>([](GtkApplication* app,gpointer data) -> void{
        Application* application = static_cast<Application*>(data);
        for(auto it = application->m_shutdownCallbacks.begin();it != application->m_shutdownCallbacks.end(); ++it){
            it->first(app,it->second);
        }

        //  Finally quiting the Application not 'GApplication'
        //  So that the application can be closed properly after quiting the 'GApplication'
        application->quit();
    })),this);
}

/**
 * @brief Preboots the application
 */
void Application::preboot(){
    //  Setting prebooted state to true and firing 'application.preboot' event
    this->m_prebooted = true;
    this->m_eventDispatcher->fire("application.preboot");
}

/**
 * @brief Boots the application
 */
void Application::boot(){
    // Setting booted state to true and firing 'application.boot' event
    this->m_booted = true;
    this->m_eventDispatcher->fire("application.boot");
}

/**
 * @brief Quits the application after the 'GApplication' fired 'shutdown' event
 */
void Application::quit(){
    //  Before deleting the application itself, 'application.quit' event is fired.
    //  This event secures any remaining task to be done before application is quitted.
    Application::getInstance()->m_eventDispatcher->fire("application.quit");
    delete m_instance;
    m_instance = nullptr;
}

/**
 * @brief Boots up the application
 */
void Application::bootUp(){
    //  Creating the 'GtkApplication' before the application actually boots
    this->m_GtkApp = gtk_application_new(this->m_appId.c_str(),this->m_flags);
    g_message("Application: GtkApplication created");

    //  Setting 'activate' callback for the 'GtkApplication'
    //  so that some configuration can be carried out.
    this->setActivateCallback([&](GtkApplication* gapp,gpointer data){
        g_message("Application: GtkApplication activated");
        Application* app = static_cast<Application*>(data);

        //  Creating a defualt 'GtkWindow' from the 'GtkApplication'
        //  This window is set with default title of the application's name
        GtkWidget* window = gtk_application_window_new(gapp);
        gtk_window_set_title(GTK_WINDOW(window),app->getAppName().c_str());
        //  Centering the window
        gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
        //  Maximizing the window
        gtk_window_maximize(GTK_WINDOW(window));

        //  Getting the 'windowId' of the window created before
        guint window_id = gtk_application_window_get_id(GTK_APPLICATION_WINDOW(window));
        g_message("Default Stage Id: %d",window_id);

        //  Setting default stage to the window created above
        app->getViewManager()->setDefaultStage(window_id);
    },this);

    //  Finally regisert the Base GApplicationSignals to the 'GtkApplicaton'
    //  So that the the base signals are properly handled
    this->registerBaseGApplicationSignals();

    //  And boot the application
    this->boot();
}

/**
 * @brief Runs the application
 *
 * @param argc number of command-line arguments
 * @param argv array of command-line arguments
 *
 * @return int
 */
int Application::run(int &argc,char** &argv){
    //  If the application is not booted boot up the application
    if(!isBooted()){
        this->bootUp();
    }

    //  Loads default controller
    loadDefaultController();

    //  Finally runs the GApplication
    return g_application_run(G_APPLICATION(this->m_GtkApp),argc,argv);
}

/**
 * @brief Loads default controller
 */
void Application::loadDefaultController(){
    g_message("Application: Setting Up Default Controller");

    //  After the 'GApplication' is activated the controller manager
    //  loads the default controller provided into the configuration
    this->setActivateCallback([](GtkApplication* app,gpointer data){
        g_message("Application: Trying to load Controller after activated");
        ControllerManager* manager = (ControllerManager*) (data);
        manager->loadController(ConfigManagerFacade::get()->getDefaultController());
    },this->m_controllerManager);

    g_message("Application: loadDefaultController() closing");
}

/**
 * @brief Runs all the callbacks for application preboot event
 *
 * @param publisher The event publisher object passed for the callback
 */
void Application::onPreboot(IEventPublisher* publisher){
    g_message("Application: Prebooted");
    for(auto it: this->m_prebootCallbacks){
        it(static_cast<Application*>(publisher));
    }
}

/**
 * @brief Runs all the callbacks for the application boot event
 *
 * @param publisher The event publisher object passed for the callback
 */
void Application::onBoot(IEventPublisher* publisher){
    g_message("Application: Booted");
    for(auto it: this->m_bootCallbacks){
        it(static_cast<Application*>(publisher));
    }
}

/**
 * @brief Runs all the callbacks for the application quit event
 *
 * @param publisher The event publisher object passed for the callback
 */
void Application::onQuit(IEventPublisher* publisher){
    g_message("Application: Quiting");
    for(auto it: this->m_quitCallbacks){
        it(static_cast<Application*>(publisher));
    }
}

/**
 * @brief Registers preboot callback to the list of preboot callbacks
 *
 * @param prebootCallback Function object for the callback
 */
void Application::setPrebootCallback(ApplicationEventCallback prebootCallback){
    this->m_prebootCallbacks.push_back(prebootCallback);
}

/**
 * @brief Registers boot callback to the list of boot callbacks
 *
 * @param bootCallback Function object for the callback
 */
void Application::setBootCallback(ApplicationEventCallback bootCallback){
    this->m_bootCallbacks.push_back(bootCallback);
}

/**
 * @brief Registers quit callback to the list of quit callbacks
 *
 * @param quitCallback Function object for the callback
 */
void Application::setQuitCallback(ApplicationEventCallback quitCallback){
    this->m_quitCallbacks.push_back(quitCallback);
}

/**
 * @brief Registers the activate callback to the list
 *
 * @param activatedCallback Function object for the callback
 * @param data Any additional data for the callback
 */
void Application::setActivateCallback(ApplicationCallback activatedCallback,gpointer data){
    this->m_activateCallbacks.insert({activatedCallback,data});
}

/**
 * @brief Registers the shutdown callback to the list
 *
 * @param activatedCallback Function object for the callback
 * @param data Any additional data for the callback
 */
void Application::setShutdownCallback(ApplicationCallback shutdownCallback,gpointer data){
    this->m_shutdownCallbacks.insert({shutdownCallback,data});
}

/**
 * @brief Loads configuration from the config repository
 *
 * @param configRepo ConfigRepository object to load configurations from
 */
void Application::loadConfiguration(ConfigRepository* configRepo){
    g_message("Application: Loading loadConfiguration");
    //  Firstly getting the configuration document from the configuration repository
    rapidjson::Document *configDocument = configRepo->getConfigDocument();

    //  Setting up event listener for config loaded event
    //  After the config is loaded the config repository is deleted and set to nullptr
    this->m_eventDispatcher->listen("application.config.loaded",[&](IEventPublisher* publisher){
        delete configRepo;
        configRepo = nullptr;
    },this->m_configManager);

    //  Finally handing over the config document to config manager
    //  So that it can load the configurations
    this->m_configManager->loadConfigDocument(configDocument);
}

/**
 * @brief Loads providers from the provider repository
 *
 * @param providerRepo ProviderRepository object from which the providers are to be loaded
 */
void Application::loadProviders(ProviderRepository* providerRepo){
    this->m_serviceProviderManager->loadProviders(providerRepo);
}

/**
 * @brief Loads Controllers from controller Repository
 *
 * @param controllerRepo ControllerRepository object from which the controllers are to be loaded
 */
void Application::loadControllers(ControllerRepository* controllerRepo){
    this->m_controllerManager->loadControllers(controllerRepo);
}

}
