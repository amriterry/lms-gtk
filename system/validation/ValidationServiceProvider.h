#ifndef _VALIDATION_SERVICE_PROVIDER_H_
#define _VALIDATION_SERVICE_PROVIDER_H_

#include <system/foundation/ServiceProvider.h>

using namespace std;

namespace tuber{

class ValidationServiceProvider: public ServiceProvider{
public:
	ValidationServiceProvider(Container* container) : ServiceProvider(container){}
	void registerService();
	string provides();
};

#endif