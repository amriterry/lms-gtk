#ifndef _VALIDATOR_H_
#define _VALIDATOR_H_

#include <system/facades/Facade.h>

#include <system/validation/ValidationFactory.h>

namespace tuber{

class Validator: public Facade{
public:
	static ValidationFactory* get(){
		return m_app->resolve<ValidationFactory>(getFacadeAccessor());
	}

	static string getFacadeAccessor(){
        return "ValidationFactory";
	}
};

}



#endif
