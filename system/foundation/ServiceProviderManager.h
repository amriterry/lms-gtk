#ifndef _SERVICE_PROVIDER_MANAGER_H_
#define _SERVICE_PROVIDER_MANAGER_H_

#include <vector>

#include <system/support/IBindable.h>

using namespace std;

namespace tuber{

class Application;
class ProviderRepository;
class ServiceProvider;

class ServiceProviderManager : public IBindable{
public:
	ServiceProviderManager(Application* app);
	virtual ~ServiceProviderManager();

	void loadProviders(ProviderRepository* providerRepo);
	void registerProvider(ServiceProvider* provider);
protected:

    bool isProviderRegistered(ServiceProvider* provider);
    void markAsRegistered(ServiceProvider* provider);
    void destroyProviders();

	Application* m_app;
	vector<ServiceProvider*> m_serviceProviders;
};

}

#endif //_SERVICE_PROVIDER_MANAGER_H_
