#ifndef _MAIN_CONTROLLER_H_
#define _MAIN_CONTROLLER_H_

#include <system/controller/Controller.h>

using namespace tuber;

class MainController : public Controller{
public:
    void onCreate(Bundle* package);
    void onPause();
};

#endif //_MAIN_CONTROLLER_H_
