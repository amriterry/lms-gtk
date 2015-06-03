#ifndef _CONTROLLER_LIFE_MANAGER_FACADE_H_
#define _CONTROLLER_LIFE_MANAGER_FACADE_H_

#include <system/facades/Facade.h>

#include <system/controller/ControllerLifeCycleManager.h>

namespace tuber{

class ControllerLifeManagerFacade: public Facade{
public:
	static ControllerLifeCycleManager* get(){
		return m_app->resolve<ControllerLifeCycleManager>(getFacadeAccessor());
	}

	static string getFacadeAccessor(){
        return "ControllerLifeCycleManager";
	}
};

}

#endif
