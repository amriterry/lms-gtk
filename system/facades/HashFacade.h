#ifndef _HASH_FACADE_H_
#define _HASH_FACADE_H_

#include <system/facades/Facade.h>

#include <system/hash/HashFactory.h>

namespace tuber{

class HashFacade: public Facade{
public:
	static HashFactory* get(){
		return m_app->resolve<HashFactory>(getFacadeAccessor());
	}

	static string getFacadeAccessor(){
        return "HashFactory";
	}
};

}



#endif
