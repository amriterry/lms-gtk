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

/**
 * @brief Manages the controller
 * 
 * @param controller Controller to be managed
 * 
 * @todo [No complete body]
 */
void ControllerLifeCycleManager::manage(Controller* controller){
	g_message("ControllerLifeCycleManager: Loading controller");
	this->m_controllerStack.push_back(controller);
	//controller->start();
}

/**
 * @brief Responds to the request by creating creating controller and starting its life
 * 
 * @param controllerRequest The request object to be parsed
 * @param clientController The controller which sent the request
 */
void ControllerLifeCycleManager::respondRequest(Request* controllerRequest,Controller* clientController){
	//	Resolving the controller out of the container
	string controllerKey = controllerRequest->getActionKey();
	Bundle* package = controllerRequest->getPackage();
	Controller* controller = this->m_app->resolve<Controller>(controllerKey);

	//	Initializing controller
	this->initializeController(controller);

	//	If the client controller is not set 
	//	then we set the relationship between parent and child controller
	//	Also we pause the parent controller
	if(clientController != nullptr){
		clientController->setChildController(controller);
		controller->setParentController(clientController);
		this->pauseController(clientController);
	}

	//	Starting the life of the controller
	this->startLife(controller,package);
}

/**
 * @brief Pauses the controller provided
 * 
 * @param controller pauses the controller
 * 
 * @todo Change the position of the controller in the controller list
 */
void ControllerLifeCycleManager::pauseController(Controller* controller){
	this->pause(controller);
	//...change the position of controller in the vector
}

/**
 * @brief Initializes controller by injecting controller dependencies
 * 
 * @param controller Controller to initialize
 */
void ControllerLifeCycleManager::initializeController(Controller* controller){
	controller->setControllerDependencies(this->m_app,this->m_app->resolve<ControllerStateManager>("ControllerStateManager"),this->m_app->getViewManager());
}

/**
 * @brief Starts the life of the controller
 * 
 * @param controller Controller whose life is to be started
 * @param package Package to inject into the controller when the controller is 'created'
 */
void ControllerLifeCycleManager::startLife(Controller* controller,Bundle* package){
	this->m_controllerStack.push_back(controller);
	this->create(controller,package);
	this->start(controller);
}

/**
 * @brief Ends the life of the controller
 * 
 * @param controller Controller whose life is to be ended
 */
void ControllerLifeCycleManager::endLife(Controller* controller){
	//	Checking if the controller is paused
	//	If is not paused then first pause the controller
	//	then only stop the controller
	if(!controller->getStateManager()->isPaused()){
        this->pause(controller);
	}
	this->stop(controller);

	//	Removing the controller from stack
	int position = find(m_controllerStack.begin(),m_controllerStack.end(),controller) - m_controllerStack.begin();
    m_controllerStack.erase(m_controllerStack.begin() + position);
}

/**
 * @brief 'Creates' the controller and injects the package to the controller
 * 
 * @param controller Controller to be 'created'
 * @param package Package to be injected
 */
void ControllerLifeCycleManager::create(Controller* controller,Bundle* package){
	//	Changing state of the controller
	controller->getStateManager()->setState(ControllerState::CREATE);
	//	Calling onCreate method of the controller
	controller->onCreate(package);
	//	Firing is any event exists for the $(controller).create
	this->m_app->getEventDispatcher()->fireIfExists(controller->getBindingKey() + ".create");
}

/**
 * @brief 'Starts' the controller
 * 
 * @param controller Controller to be 'started'
 */
void ControllerLifeCycleManager::start(Controller* controller){
	controller->getStateManager()->setState(ControllerState::START);
	controller->onStart();
	this->m_app->getEventDispatcher()->fireIfExists(controller->getBindingKey() + ".start");
}

/**
 * @brief 'Resumes' the controller
 * 
 * @param controller Controller to be 'resumed'
 */
void ControllerLifeCycleManager::resume(Controller* controller){
	controller->getStateManager()->setState(ControllerState::RESUME);
	controller->onResume();
	this->m_app->getEventDispatcher()->fireIfExists(controller->getBindingKey() + ".resume");
}

/**
 * @brief 'Pauses' controller
 * 
 * @param controller Controller to be 'paused'
 */
void ControllerLifeCycleManager::pause(Controller* controller){
	controller->getStateManager()->setState(ControllerState::PAUSE);
	controller->onPause();
	this->m_app->getEventDispatcher()->fireIfExists(controller->getBindingKey() + ".pause");
}

/**
 * @brief 'Stops' controller
 * 
 * @param controller Controller to be 'stoped'
 */
void ControllerLifeCycleManager::stop(Controller* controller){
	controller->getStateManager()->setState(ControllerState::STOP);
	controller->onStop();
	this->m_app->getEventDispatcher()->fireIfExists(controller->getBindingKey() + ".stop");
}

}
