#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <functional>
#include <string>
#include <vector>

#include <gtk/gtk.h>

#include <system/foundation/Container.h>
#include <system/support/IEventPublisher.h>

namespace tuber{

class Application;
class ServiceProvider;
class ConfigRepository;
class ProviderRepository;
class ControllerRepository;
class ConfigManager;
class ControllerManager;
class ServiceProviderManager;
class EventDispatcher;
class ViewManager;
class PathManager;

typedef void (*ApplicationCallback)(GtkApplication*,gpointer);
typedef function<void(Application*)> ApplicationEventCallback;

/** \class Application
 * \brief Main Application Class
 * \details The main application class stores every object resolved out of the container and
 *          handles all object creation and deletion process automatically. So any object resolved
 *          out of container should not be deleted manually.
 *
 */

class Application : public Container, public IEventPublisher{
public:
    //Application getters
    string getAppName();
    GtkApplication* getGtkApp();

    ConfigManager* getConfigManager();
    ControllerManager* getControllerManager();
    ServiceProviderManager* getServiceProviderManager();

    EventDispatcher* getEventDispatcher();
    ViewManager* getViewManager();

    static Application* getInstance();

    //Application state info methods
    bool isPrebooted();
    bool isBooted();

    //Application after preboot setup methods
    void loadConfiguration(ConfigRepository* configRepo);
    void loadProviders(ProviderRepository* providerRepo);
    void loadControllers(ControllerRepository* controllerRespo);

    //Application EventRegistering Methods
    void setPrebootCallback(ApplicationEventCallback prebootCallback);
    void setBootCallback(ApplicationEventCallback bootCallback);
    void setQuitCallback(ApplicationEventCallback quitCallback);
    void setActivateCallback(ApplicationCallback activateCallback,gpointer data = NULL);
    void setShutdownCallback(ApplicationCallback shutdownCallback,gpointer data = NULL);

    //Application Run method
    int run(int &argc,char** &argv);

    //Application Creating Method
    static Application* create(string appId,string appName,GApplicationFlags flags = G_APPLICATION_FLAGS_NONE);

    //Application Quiting Method
    static void quit();
protected:
    //Application ctor
    Application(string appId,string appName,GApplicationFlags flags);

    //Application Destructor
    virtual ~Application();

    //Application dependencies resolving methods
    void resolveManagerDependencies();
    void resolveServiceDependencies();

    //Application base configuration methods
    void registerBasePaths();
    void registerBaseManagers();
    void registerBaseServiceProviders();
    void registerBaseApplicationEvents();
    void registerBaseGApplicationSignals();

    //Application Controller methods
    void loadDefaultController();

    //Application ServiceProvider registering methods
    /*void registerProvider(ServiceProvider* provider);
    bool isProviderRegistered(ServiceProvider* provider);
    void markAsRegistered(ServiceProvider* provider);*/

    //Application Controller registering methods
    /*void registerController(string controllerKey,ControllerResolvingCallback callback);
    bool isControllerRegistered(string controllerKey);
    void markControllerAsRegistered(string controllerKey);*/

    //Application life state methods
    void onPreboot(IEventPublisher* publisher);
    void onBoot(IEventPublisher* publisher);
    void onQuit(IEventPublisher* publisher);

    void preboot();
    void boot();

    //Application boot up method
    void bootUp();

    string m_appId;
    string m_appName;
    bool m_prebooted;
    bool m_booted;

    GtkApplication* m_GtkApp;
    GApplicationFlags m_flags;

    PathManager* m_pathManager;
    ConfigManager* m_configManager;
    ControllerManager* m_controllerManager;
    ServiceProviderManager* m_serviceProviderManager;

    EventDispatcher* m_eventDispatcher;
    ViewManager* m_viewManager;

    //vector<string> m_controllers;
    //vector<ServiceProvider*> m_serviceProviders;
    vector<ApplicationEventCallback> m_prebootCallbacks;
    vector<ApplicationEventCallback> m_bootCallbacks;
    vector<ApplicationEventCallback> m_quitCallbacks;
    map<ApplicationCallback,gpointer> m_activateCallbacks;
    map<ApplicationCallback,gpointer> m_shutdownCallbacks;

    static Application* m_instance;
private:
    Application(Application const&);
    Application& operator=(Application const&);
};

}

#endif //_APPLICATION_H_
