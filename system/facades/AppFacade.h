#ifndef _APP_FACADE__H_
#define _APP_FACADE_H_

#include <system/facades/Facade.h>

namespace tuber{

class AppFacade: public Facade{
public:
	static Application* get(){
		return m_app;
	}

	static string getFacadeAccessor(){
        return "Application";
	}
};

}



#endif
