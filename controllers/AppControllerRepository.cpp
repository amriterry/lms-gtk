#include <controllers/AppControllerRepository.h>
#include <system/controller/ControllerStateManager.h>
#include <system/foundation/Application.h>
#include <system/views/ViewManager.h>

#include <system/facades/App.h>

#include "MainController.h"
#include "TestController.h"
#include "SplashController.h"
#include "LoginController.h"
#include "SaveBookController.h"
#include "IssueBookController.h"
#include "SavePublicationController.h"
#include "SaveAuthorController.h"
#include "SaveBookCopiesController.h"
#include "UnissueBookCopyController.h"
#include "SaveUserController.h"

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
    this->m_controllers.insert({
        "SaveBookController",
        [&](Container* app) -> IBindable*{
            return new SaveBookController();
        }
    });
    this->m_controllers.insert({
        "IssueBookController",
        [&](Container* app) -> IBindable*{
            return new IssueBookController();
        }
    });
    this->m_controllers.insert({
        "SavePublicationController",
        [&](Container* app) -> IBindable*{
            return new SavePublicationController();
        }
    });
    this->m_controllers.insert({
        "SaveAuthorController",
        [&](Container* app) -> IBindable*{
            return new SaveAuthorController();
        }
    });
    this->m_controllers.insert({
        "SaveBookCopiesController",
        [&](Container* app) -> IBindable*{
            return new SaveBookCopiesController();
        }
    });
    this->m_controllers.insert({
        "UnissueBookCopyController",
        [&](Container* app) -> IBindable*{
            return new UnissueBookCopyController();
        }
    });
    this->m_controllers.insert({
        "SaveUserController",
        [&](Container* app) -> IBindable*{
            return new SaveUserController();
        }
    });
}

map<string,ControllerResolvingCallback>	AppControllerRepository::getControllers(){
    return this->m_controllers;
}
