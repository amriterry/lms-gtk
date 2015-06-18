#ifndef _TEST_CONTROLLER_H_
#define _TEST_CONTROLLER_H_

#include <system/controller/Controller.h>

using namespace tuber;

class TestController: public Controller{
public:
	void onCreate(Bundle* package);
	void onStart();
};

#endif
