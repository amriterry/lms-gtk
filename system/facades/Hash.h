#ifndef _HASH_H_
#define _HASH_H_

#include <system/facades/Facade.h>

#include <system/hash/HashFactory.h>

namespace tuber{

class Hash: public Facade{
public:
	static HashFactory* get(){
		return m_app->resolve<HashFactory>(getFacadeAccessor());
	}

	static string getFacadeAccessor(){
        return "HashFactory";
	}

	static string make(string strToEncrypt){
		return get()->encrypt(strToEncrypt);
	}
};

}



#endif
