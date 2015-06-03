#ifndef _CONTROLLER_REPOSITORY_H_
#define _CONTROLLER_REPOSITORY_H_

#include <map>
#include <string>
#include <functional>

using namespace std;

namespace tuber{

class IBindable;
class Controller;
class Container;
class Application;

typedef function<IBindable*(Container*)> ControllerResolvingCallback;

class ControllerRepository{
public:
	virtual ~ControllerRepository(){}
	virtual map<string,ControllerResolvingCallback>	getControllers() = 0;
protected:
	ControllerRepository(Application* app): m_app(app){}
	Application* m_app;
};

}

#endif //_CONTROLLER_REPOSITORY_H_
