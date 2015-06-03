#include <gtk/gtk.h>
#include <vector>
#include <algorithm>

#include <system/foundation/ServiceProviderManager.h>
#include <system/foundation/ServiceProvider.h>
#include <system/foundation/ProviderRepository.h>

using namespace std;

namespace tuber{

ServiceProviderManager::ServiceProviderManager(Application* app) : m_app(app){
	g_message("ServiceProviderManager: Creating ServiceProviderManager");
}

ServiceProviderManager::~ServiceProviderManager(){
	g_message("ServiceProviderManager: Deleting ServiceProviderManager");
	for(vector<ServiceProvider*>::reverse_iterator it = m_serviceProviders.rbegin();it != m_serviceProviders.rend();++it){
        delete *it;
    }

	delete m_providerRepo;
}

void ServiceProviderManager::loadProviders(ProviderRepository* providerRepo){
	g_message("ServiceProviderManager: Loading Providers");
	this->m_providerRepo = providerRepo;

	vector<ServiceProvider*> providers = providerRepo->getProviders();
    for(unsigned int i = 0;i < providers.size(); ++i){
        this->registerProvider(providers[i]);
    }
}

void ServiceProviderManager::registerProvider(ServiceProvider* provider){
	g_message("ServiceProviderManager: Registering ServiceProvider that provides \"%s\"",provider->provides().c_str());
	if(isProviderRegistered(provider)){
        return;
    }
    provider->registerService();

    this->markAsRegistered(provider);
}

bool ServiceProviderManager::isProviderRegistered(ServiceProvider* provider){
	return (find(this->m_serviceProviders.begin(),this->m_serviceProviders.end(),provider) != this->m_serviceProviders.end());
}

void ServiceProviderManager::markAsRegistered(ServiceProvider* provider){
	this->m_serviceProviders.push_back(provider);
}

}
