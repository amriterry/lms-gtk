#include <controllers/AppControllerRepository.h>
#include <system/controller/ControllerStateManager.h>
#include <system/foundation/Application.h>
#include <system/views/ViewManager.h>

#include <system/facades/App.h>

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
            return new SplashController();
        }
    });
    this->m_controllers.insert({
        "MainController",
        [&](Container *app) -> IBindable* {
            return new MainController();
        }
    });
    this->m_controllers.insert({
        "TestController",
        [&](Container *app) -> IBindable* {
            return new TestController();
        }
    });
    this->m_controllers.insert({
        "LoginController",
        [&](Container *app) -> IBindable* {
            return new LoginController();
        }
    });
}

map<string,ControllerResolvingCallback>	AppControllerRepository::getControllers(){
    return this->m_controllers;
}