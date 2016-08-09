#ifndef _AUTH_SERVICE_PROVIDER_H_
#define _AUTH_SERVICE_PROVIDER_H_

#include <system/foundation/ServiceProvider.h>

namespace tuber{

class AuthServiceProvider : public ServiceProvider{
public:
	AuthServiceProvider(Container* container) : ServiceProvider(container){}
	void registerService();
	string provides();
};

}

#endif