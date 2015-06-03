#ifndef _TEST_CONTROLLER_H_
#define _TEST_CONTROLLER_H_

#include <system/controller/Controller.h>

using namespace tuber;

class TestController: public Controller{
public:
	TestController(Application* app,ControllerStateManager* stateManager,ViewManager* viewManager) : Controller(app,stateManager,viewManager){}
	void onCreate(Bundle* package);
};

#endif