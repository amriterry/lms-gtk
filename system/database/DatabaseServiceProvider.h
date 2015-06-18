#ifndef _DATABASE_SERVICE_PROVIDER_H_
#define _DATABASE_SERVICE_PROVIDER_H_

#include <system/foundation/ServiceProvider.h>

namespace tuber{

class DatabaseServiceProvider : public ServiceProvider{
public:
	DatabaseServiceProvider(Container* container) : ServiceProvider(container){}
	void boot();
	void registerService();
	string provides();
};

}

#endif