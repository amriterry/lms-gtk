#ifndef _BUNDLE_FACADE_H_
#define _BUNDLE_FACADE_H_

#include <system/facades/Facade.h>

#include <system/bundle/Bundle.h>

namespace tuber{

class BundleFacade: public Facade{
public:
	static Bundle* get(){
		return m_app->resolve<Bundle>(getFacadeAccessor());
	}

	static string getFacadeAccessor(){
        return "Bundle";
	}
};

}



#endif
