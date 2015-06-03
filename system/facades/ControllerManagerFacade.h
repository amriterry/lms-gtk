#ifndef _CONTROLLER_MANAGER_FACADE_H_
#define _CONTROLLER_MANAGER_FACADE_H_

#include <system/facades/Facade.h>

#include <system/controller/ControllerManager.h>

namespace tuber{

class ControllerManagerFacade: public Facade{
public:
	static ControllerManager* get(){
		return m_app->resolve<ControllerManager>(getFacadeAccessor());
	}

	static string getFacadeAccessor(){
        return "ControllerManager";
	}
};

}

#endif
