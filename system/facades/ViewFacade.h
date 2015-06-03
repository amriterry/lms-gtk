#ifndef _VIEW_FACADE_H_
#define _VIEW_FACADE_H_

#include <system/facades/Facade.h>

#include <system/views/ViewManager.h>

namespace tuber{

class ViewFacade: public Facade{
public:
	static ViewManager* get(){
		return m_app->resolve<ViewManager>(getFacadeAccessor());
	}

	static string getFacadeAccessor(){
        return "ViewManager";
	}
};

}



#endif
