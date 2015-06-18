#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <string>
#include <vector>
#include <functional>

#include <gtk/gtk.h>

#include <system/foundation/Container.h>
#include <system/support/IEventPublisher.h>

namespace tuber{

class Application;
class ViewManager;
class PathManager;
class ConfigManager;
class ServiceProvider;
class EventDispatcher;
class ConfigRepository;
class ControllerManager;
class ProviderRepository;
class ControllerRepository;
class ServiceProviderManager;

//  Aliasing GtkApplication callback to ApplicationCallback
typedef void (*ApplicationCallback)(GtkApplication*,gpointer);
//  Aliasing event callback for Application to ApplicationEventCallback
typedef function<void(Application*)> ApplicationEventCallback;

/**
 * @class Application
 * @brief Main Application Class
 * @details The main application class stores every object resolved out of the container and
 *          handles all object creation and deletion process automatically. So any object resolved
 *          out of container should not be deleted manually.
 */

class Application : public Container, public IEventPublisher{
public:
    //Application getters
    string getAppName();
    GtkApplication* getGtkApp();

    PathManager* getPathManager();
    ViewManager* getViewManager();
    ConfigManager* getConfigManager();
    EventDispatcher* getEventDispatcher();
    ControllerManager* getControllerManager();
    ServiceProviderManager* getServiceProviderManager(); 


    //Application state info methods
    bool isPrebooted();
    bool isBooted();

    //Application after preboot setup methods
    void loadConfiguration(ConfigRepository* configRepo);
    void loadProviders(ProviderRepository* providerRepo);
    void loadControllers(ControllerRepository* controllerRespo);

    //Application EventRegistering Methods
    void setBootCallback(ApplicationEventCallback bootCallback);
    void setQuitCallback(ApplicationEventCallback quitCallback);
    void setPrebootCallback(ApplicationEventCallback prebootCallback);
    void setActivateCallback(ApplicationCallback activateCallback,gpointer data = NULL);
    void setShutdownCallback(ApplicationCallback shutdownCallback,gpointer data = NULL);

    //Application Run method
    int run(int &argc,char** &argv);

    static Application* getInstance();

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

    //Application life state methods
    void onBoot(IEventPublisher* publisher);
    void onQuit(IEventPublisher* publisher);
    void onPreboot(IEventPublisher* publisher);

    void boot();
    void bootUp();
    void preboot();

    //Application boot up method

    bool m_booted;
    bool m_prebooted;
    string m_appId;
    string m_appName;

    GtkApplication* m_GtkApp;
    GApplicationFlags m_flags;

    ViewManager* m_viewManager;
    PathManager* m_pathManager;
    ConfigManager* m_configManager;
    EventDispatcher* m_eventDispatcher;
    ControllerManager* m_controllerManager;
    ServiceProviderManager* m_serviceProviderManager;

    vector<ApplicationEventCallback> m_bootCallbacks;
    vector<ApplicationEventCallback> m_quitCallbacks;
    vector<ApplicationEventCallback> m_prebootCallbacks;
    map<ApplicationCallback,gpointer> m_activateCallbacks;
    map<ApplicationCallback,gpointer> m_shutdownCallbacks;

    static Application* m_instance;
private:
    Application(Application const&);
    Application& operator=(Application const&);
};

}

#endif //_APPLICATION_H_
