#ifndef _APP__H_
#define _APP_H_

#include <system/facades/Facade.h>

namespace tuber{

class App: public Facade{
public:
	static Application* get(){
		return m_app;
	}

	static string getFacadeAccessor(){
        return "Application";
	}

	template<typename T>
	static T* resolve(string key){
		return m_app->resolve<T>(key);
	}

	static void destroy(IBindable* bindable){
		m_app->destroyBindable(bindable);
		bindable = nullptr;
	}
};

}



#endif
