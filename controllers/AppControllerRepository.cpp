#include <controllers/AppControllerRepository.h>
#include <system/controller/ControllerStateManager.h>
#include <system/foundation/Application.h>
#include <system/views/ViewManager.h>

#include <system/facades/AppFacade.h>

#include "MainController.h"
#include "TestController.h"
#include "SplashController.h"
#include "LoginController.h"


using namespace std;
using namespace tuber;

AppControllerRepository::AppControllerRepository(Application* app) : ControllerRepository(app){

    this->m_controllers.insert({
        "SplashController",
        [&](Container* app) -> IBindable* {
            ControllerStateManager* stateManager = app->resolve<ControllerStateManager>("ControllerStateManager");
            ViewManager* viewManager = app->resolve<ViewManager>("ViewManager");
            return new SplashController(AppFacade::get(),stateManager,viewManager);
        }
    });
    this->m_controllers.insert({
        "MainController",
        [&](Container *app) -> IBindable* {
            ControllerStateManager* stateManager = app->resolve<ControllerStateManager>("ControllerStateManager");
            ViewManager* viewManager = app->resolve<ViewManager>("ViewManager");
            return new MainController(AppFacade::get(),stateManager,viewManager);
        }
    });
    this->m_controllers.insert({
        "TestController",
        [&](Container *app) -> IBindable* {
            ControllerStateManager* stateManager = app->resolve<ControllerStateManager>("ControllerStateManager");
            ViewManager* viewManager = app->resolve<ViewManager>("ViewManager");
            return new TestController(AppFacade::get(),stateManager,viewManager);
        }
    });
    this->m_controllers.insert({
        "LoginController",
        [&](Container *app) -> IBindable* {
            ControllerStateManager* stateManager = app->resolve<ControllerStateManager>("ControllerStateManager");
            ViewManager* viewManager = app->resolve<ViewManager>("ViewManager");
            return new LoginController(AppFacade::get(),stateManager,viewManager);
        }
    });
}

map<string,ControllerResolvingCallback>	AppControllerRepository::getControllers(){
    return this->m_controllers;
}
