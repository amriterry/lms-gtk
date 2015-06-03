#include <gtk/gtk.h>

#include <algorithm>

#include <system/controller/ControllerLifeCycleManager.h>
#include <system/request/Request.h>
#include <system/controller/Controller.h>
#include <system/foundation/Application.h>
#include <system/controller/ControllerState.h>
#include <system/controller/ControllerStateManager.h>
#include <system/events/EventDispatcher.h>

#include <controllers/MainController.h>

using namespace std;

namespace tuber{

void ControllerLifeCycleManager::manage(Controller* controller){
	g_message("ControllerLifeCycleManager: Loading controller");
	this->m_controllerStack.push_back(controller);
	//controller->start();
}

void ControllerLifeCycleManager::respondRequest(Request* controllerRequest,Controller* clientController){
	//..respond to request
	string controllerKey = controllerRequest->getActionKey();
	g_message("ControllerLifeCycleManager: Responding request for %s",controllerKey.c_str());
	Bundle* package = controllerRequest->getPackage();
	Controller* controller = this->m_app->resolve<Controller>(controllerKey);
	g_message("ControllerLifeCycleManager: Resolved controller of %s",controller->getBindingKey().c_str());

	if(clientController != nullptr){
		clientController->setChildController(controller);
		controller->setParentController(clientController);
		this->pauseController(clientController);
	}
	this->startLife(controller,package);
}

void ControllerLifeCycleManager::pauseController(Controller* controller){
	this->pause(controller);
	//...change the position of controller in the vector
}

void ControllerLifeCycleManager::startLife(Controller* controller,Bundle* package){
	this->m_controllerStack.push_back(controller);
	this->create(controller,package);
	this->start(controller);
}

void ControllerLifeCycleManager::endLife(Controller* controller){
	g_message("ControllerLifeCycleManager: ending life of %s",controller->getBindingKey().c_str());
	if(!controller->getStateManager()->isPaused()){
        this->pause(controller);
	}
	this->stop(controller);

	//	Removing the controller from stack
	int position = find(m_controllerStack.begin(),m_controllerStack.end(),controller) - m_controllerStack.begin();
    m_controllerStack.erase(m_controllerStack.begin() + position);
}

void ControllerLifeCycleManager::create(Controller* controller,Bundle* package){
	controller->getStateManager()->setState(ControllerState::CREATE);
	controller->onCreate(package);
	this->m_app->getEventDispatcher()->fireIfExists(controller->getBindingKey() + ".create");
}

void ControllerLifeCycleManager::start(Controller* controller){
	controller->getStateManager()->setState(ControllerState::START);
	controller->onStart();
	this->m_app->getEventDispatcher()->fireIfExists(controller->getBindingKey() + ".start");
}

void ControllerLifeCycleManager::resume(Controller* controller){
	controller->getStateManager()->setState(ControllerState::RESUME);
	controller->onResume();
	this->m_app->getEventDispatcher()->fireIfExists(controller->getBindingKey() + ".resume");
}

void ControllerLifeCycleManager::pause(Controller* controller){
	controller->getStateManager()->setState(ControllerState::PAUSE);
	controller->onPause();
	this->m_app->getEventDispatcher()->fireIfExists(controller->getBindingKey() + ".pause");
}

void ControllerLifeCycleManager::stop(Controller* controller){
	controller->getStateManager()->setState(ControllerState::STOP);
	controller->onStop();
	this->m_app->getEventDispatcher()->fireIfExists(controller->getBindingKey() + ".stop");
}

}
