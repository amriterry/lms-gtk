#ifndef _REQUEST_FACADE_H_
#define _REQUEST_FACADE_H_

#include <system/facades/Facade.h>

#include <system/request/Request.h>

namespace tuber{

class RequestFacade: public Facade{
public:
	static Request* get(){
		return m_app->resolve<Request>(getFacadeAccessor());
	}

	static string getFacadeAccessor(){
        return "Request";
	}
};

}



#endif
