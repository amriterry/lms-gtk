#ifndef _BUNDLE_SERVICE_PROVIDER_H_
#define _BUNDLE_SERVICE_PROVIDER_H_

#include <system/foundation/Container.h>
#include <system/foundation/ServiceProvider.h>

namespace tuber{

class BundleServiceProvider: public ServiceProvider{
public:
	BundleServiceProvider(Container* container) : ServiceProvider(container){
    	g_message("BundleServiceProvider: Creating BundleServiceProvider");
    }
	void registerService();
	string provides();
	~BundleServiceProvider();
};

}

#endif