#ifndef _CONTROLLER_LIFE_CYCLE_MANAGER_H_
#define _CONTROLLER_LIFE_CYCLE_MANAGER_H_

#include <vector>

#include <system/support/IBindable.h>

using namespace std;

namespace tuber{

class Controller;
class Application;
class Request;
class Bundle;

class ControllerLifeCycleManager : public IBindable{
public:
	ControllerLifeCycleManager(Application* app): m_app(app){}
	void manage(Controller* controller);
	void respondRequest(Request* request,Controller* clientController);
	void pauseController(Controller* controller);
	void create(Controller* controller,Bundle* package);
	void start(Controller* controller);
	void resume(Controller* controller);
	void pause(Controller* controller);
	void stop(Controller* controller);

	void endLife(Controller* controller);
protected:
	void startLife(Controller* controller,Bundle* package);
	void initializeController(Controller* controller);
private:
	vector<Controller*> m_controllerStack;
	Application* m_app;
};

}

#endif //_CONTROLLER_LIFE_CYCLE_MANAGER_H_
