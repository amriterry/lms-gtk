#ifndef _HASH_SERVICE_PROVIDER_H_
#define _HASH_SERVICE_PROVIDER_H_

#include <system/foundation/ServiceProvider.h>

namespace tuber{

class HashServiceProvider: public ServiceProvider{
public:
	HashServiceProvider(Container* container): ServiceProvider(container){}
	void registerService();
	string provides();
};

}

#endif