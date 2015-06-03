#ifndef _VIEW_SERVICE_PROVIDER_H_
#define _VIEW_SERVICE_PROVIDER_H_

#include <system/foundation/ServiceProvider.h>

namespace tuber{

class ViewServiceProvider: public ServiceProvider{
public:
	ViewServiceProvider(Container* container): ServiceProvider(container){}
	void registerService();
	string provides();
};

}

#endif