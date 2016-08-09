#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <gtk/gtk.h>

#include <system/support/IBindable.h>
#include <system/support/GAppEventCallback.h>
#include <system/controller/ControllerState.h>
#include <system/helpers/helpers.h>
#include <system/facades/Facades.h>

namespace tuber{

class Bundle;
class Request;
class Application;
class ViewManager;
class ControllerStateManager;

class Controller : public IBindable{
public:
    virtual ~Controller();
    virtual void onCreate(Bundle* package){
        g_message("\n\n*******\n\n%s: onCreate\n\n********\n\n",getBindingKey().c_str());
    }
    virtual void onStart(){
        g_message("\n\n*******\n\n%s: onStart\n\nOn Hierarchy: [%s]\n\n********\n\n",getBindingKey().c_str(),Strings::join(getHierarchy()," >> ").c_str());
    }
    virtual void onResume(Bundle* pacakge){
        g_message("\n\n*******\n\n%s: onResume\n\n********\n\n",getBindingKey().c_str());
    }
    virtual void onPause(){
        g_message("\n\n*******\n\n%s: onPause\n\n********\n\n",getBindingKey().c_str());
    }
    virtual void onStop(){
        g_message("\n\n*******\n\n%s: onStop\n\nOn Hierarchy: [%s]\n\n********\n\n",getBindingKey().c_str(),Strings::join(getHierarchy()," >> ").c_str());
    }

    void destroySceneRoot();
    void bindSceneRoot(GtkWidget* widget);
    void bindGtkBuilder(GtkBuilder* builder);

    void startController(Request* controllerRequest);
    void startController(string controllerKey,Bundle* package = nullptr);

    GtkWidget* getSceneRoot();
    GtkBuilder* getGtkBuilder();
    ViewManager* getViewManager();
    Application* getApplication();
    GtkApplication* getGtkApplication();
    GtkWidget* getSceneObj(string objName);
    GObject* getSceneRawObj(string objName);
    list<string> getHierarchy();

    ControllerStateManager* getStateManager();

    void setControllerDependencies(Application* app,ControllerStateManager* stateManager,ViewManager* viewManager);

    Controller* getChildController();
    Controller* getParentController();
    void setChildController(Controller* controller);
    void setParentController(Controller* controller);

    void unrefChildController();
    void unrefParentController();
protected:
    Controller();

    void finish(bool bindChild = false);
    void finish(Bundle* package,bool bindChild = false);

    void setTopLevelView(string viewPath);
    void setContentView(string viewPath,bool toplevel);
    void setContentView(string viewPath,string nodeName = "root",bool toplevel = false);

    bool m_builderBinded;
    bool m_rootSceneBinded;
    bool m_isSceneDestroyable;

    Controller* m_childController;
    Controller* m_parentController;

    Application* m_app;
    GtkBuilder* m_builder;
    GtkWidget* m_sceneRoot;
    ViewManager* m_viewManager;
    ControllerStateManager* m_stateManager;
};

}

#endif //_CONTROLLER_H_
