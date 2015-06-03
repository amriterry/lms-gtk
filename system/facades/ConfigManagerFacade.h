#ifndef _CONFIG_MANAGER_FACADE_H_
#define _CONFIG_MANAGER_FACADE_H_

#include <system/facades/Facade.h>

#include <system/config/ConfigManager.h>

namespace tuber{

class ConfigManagerFacade: public Facade{
public:
	static ConfigManager* get(){
		return m_app->resolve<ConfigManager>(getFacadeAccessor());
	}

	static string getFacadeAccessor(){
        return "ConfigManager";
	}
};

}

#endif
