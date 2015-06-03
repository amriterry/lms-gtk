#ifndef _APP_CONTROLLER_REPOSITORY_H_
#define _APP_CONTROLLER_REPOSITORY_H_

#include <map>
#include <string>

#include <system/controller/ControllerRepository.h>

#include <system/foundation/Container.h>

using namespace std;
using namespace tuber;

class AppControllerRepository : public ControllerRepository{
public:
    AppControllerRepository(Application* app);
    virtual map<string,ControllerResolvingCallback>	getControllers();
private:
    map<string,ControllerResolvingCallback> m_controllers;
};

#endif // _APP_CONTROLLER_REPOSITORY_H_
