#ifndef _CONTROLLER_MANAGER_H_
#define _CONTROLLER_MANAGER_H_

#include <string>
#include <vector>

#include <system/support/IBindable.h>
#include <system/controller/ControllerRepository.h>

using namespace std;

namespace tuber{

class Application;
class Request;
class ControllerLifeCycleManager;

class ControllerManager : public IBindable{
public:
	ControllerManager(Application* app,ControllerLifeCycleManager* lifeCycleManager);
	virtual ~ControllerManager();

	void loadControllers(ControllerRepository* controllerRepo);
	void loadController(string controllerKey);
	void loadController(Request* controllerRequest,Controller* clientController);

	void destroyController(Controller* controller);
protected:
	void registerController(string controllerKey,ControllerResolvingCallback callback);
    bool isControllerRegistered(string controllerKey);
    void markControllerAsRegistered(string controllerKey);

    ControllerLifeCycleManager* m_lifeCycleManager;
	ControllerRepository* m_controllerRepo;
	Application* m_app;
	vector<string> m_controllers;
};

}

#endif //_CONTROLLER_MANAGER_H_
