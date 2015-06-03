#include <string>
#include <unordered_map>
#include <algorithm>

#include <system/controller/ControllerManager.h>
#include <system/foundation/Application.h>
#include <system/foundation/Container.h>
#include <system/controller/Controller.h>
#include <system/controller/ControllerRepository.h>
#include <system/controller/ControllerLifeCycleManager.h>
#include <system/support/IBindable.h>
#include <system/request/Request.h>

using namespace std;

namespace tuber{

ControllerManager::ControllerManager(Application* app,ControllerLifeCycleManager* lifeCycleManager) : m_app(app), m_lifeCycleManager(lifeCycleManager){
	g_message("ControllerManager: Creating Controller Manager");
}

ControllerManager::~ControllerManager(){
	g_message("ControllerManager: Deleting Controller Manager");
	delete m_controllerRepo;
}

void ControllerManager::loadControllers(ControllerRepository* controllerRepo){
	g_message("ControllerManager: Loading Controllers from ControllerRepository");
	this->m_controllerRepo = controllerRepo;
	map<string,ControllerResolvingCallback> controllers = controllerRepo->getControllers();

    for(auto it = controllers.begin();it != controllers.end(); ++it){
        this->registerController(it->first,it->second);
    }
}

void ControllerManager::registerController(string controllerKey,ControllerResolvingCallback callback){
	g_message("ControllerManager: Registering Controller of key: \"%s\"",controllerKey.c_str());

	if(isControllerRegistered(controllerKey)){
        g_message("ControllerManager: controller for key[%s] already registered",controllerKey.c_str());
        return;
    }

    g_message("ControllerManager: Binding callback for %s",controllerKey.c_str());

    this->m_app->bindShared(controllerKey,callback);

    this->markControllerAsRegistered(controllerKey);
}

bool ControllerManager::isControllerRegistered(string controllerKey){
	return (find(this->m_controllers.begin(),this->m_controllers.end(),controllerKey) != this->m_controllers.end());
}

void ControllerManager::markControllerAsRegistered(string controllerKey){
	this->m_controllers.push_back(controllerKey);
}

void ControllerManager::loadController(string controllerKey){
    Request* controllerRequest = this->m_app->resolve<Request>("Request");
    controllerRequest->setActionKey(controllerKey);
    g_message("Loading controller for key: %s - %s",controllerKey.c_str(),controllerRequest->getActionKey().c_str());

    this->m_lifeCycleManager->respondRequest(controllerRequest,nullptr);
}

void ControllerManager::loadController(Request* controllerRequest,Controller* clientController){
    this->m_lifeCycleManager->respondRequest(controllerRequest,clientController);
}

void ControllerManager::destroyController(Controller* controller){
    g_message("ControllerManager: Destroying controller");
    this->m_lifeCycleManager->endLife(controller);
    this->m_app->destroyBindable(controller);
}

}
