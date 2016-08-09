#ifndef _LOGIN_CONTROLLER_H_
#define _LOGIN_CONTROLLER_H_

#include <system/controller/Controller.h>

using namespace tuber;

class LoginController : public Controller{
public:
    void onCreate(Bundle* package);
    void onPause();
};

#endif //_MAIN_CONTROLLER_H_
