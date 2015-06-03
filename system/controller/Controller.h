#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <gtk/gtk.h>

#include <system/support/IBindable.h>
#include <system/support/GAppEventCallback.h>
#include <system/controller/ControllerState.h>

namespace tuber{

class Bundle;
class Request;
class Application;
class ViewManager;
class ControllerStateManager;

class Controller : public IBindable{
public:
    virtual ~Controller();
    virtual void onCreate(Bundle* package){}
    virtual void onStart(){}
    virtual void onResume(){}
    virtual void onPause(){}
    virtual void onStop(){}

    void finish(bool bindChild = false);

    void destroySceneRoot();
    void bindSceneRoot(GtkWidget* widget);
    void bindGtkBuilder(GtkBuilder* builder);


    Controller* getChildController();
    Controller* getParentController();

    GtkWidget* getSceneRoot();
    GtkBuilder* getGtkBuilder();
    ViewManager* getViewManager();
    Application* getApplication();
    GtkApplication* getGtkApplication();
    GtkWidget* getSceneObj(string objName);
    ControllerStateManager* getStateManager();

    void startController(Request* controllerRequest);

    void setChildController(Controller* controller);
    void setParentController(Controller* controller);

    void unrefChildController();
    void unrefParentController();

    void setContentView(string viewPath,bool toplevel);
    void setContentView(string viewPath,string nodeName = "root",bool toplevel = false);
protected:
    Controller(Application* app,ControllerStateManager* stateManager,ViewManager* viewManager);

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
