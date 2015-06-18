#include <gtk/gtk.h>
#include <vector>
#include <algorithm>

#include <system/foundation/ServiceProviderManager.h>
#include <system/foundation/ServiceProvider.h>
#include <system/foundation/ProviderRepository.h>
#include <system/events/EventDispatcher.h>

using namespace std;

namespace tuber{

/**
 * @brief ServiceProviderManager 'ctor'
 * 
 * @param app Application object to bind services
 */
ServiceProviderManager::ServiceProviderManager(Application* app) : m_app(app){
	g_message("ServiceProviderManager: Creating ServiceProviderManager");
}

/**
 * @brief ServiceProviderManager destructor
 */
ServiceProviderManager::~ServiceProviderManager(){
    //  @todo   Delete the providers if previously not deleted
	g_message("ServiceProviderManager: Deleting ServiceProviderManager");
	//this->destroyProviders();
}

/**
 * @brief Destroys all the provider objects
 */
void ServiceProviderManager::destroyProviders(){
    //  Loops through each ServiceProvider objects and delete them and in LIFO order
    //  This prevents any collision between dependencies
    for(vector<ServiceProvider*>::reverse_iterator it = m_serviceProviders.rbegin();it != m_serviceProviders.rend();++it){
        delete *it;
    }
}

/**
 * @brief Loads the service providers and registers them
 * 
 * @param providerRepo Service provider repository which contains the list of service providers
 */
void ServiceProviderManager::loadProviders(ProviderRepository* providerRepo){
	g_message("ServiceProviderManager: Loading Providers");
    //  First of all registering each provider into the manager
	vector<ServiceProvider*> providers = providerRepo->getProviders();
    for(unsigned int i = 0;i < providers.size(); ++i){
        this->registerProvider(providers[i]);
    }

    //  Adding event listener to listen to application.boot event
    //  After the application is booted, boot method is called in every provider
    //  So that the service provider object can perfrom boot configurations
    //  Then delete each service provider
    this->m_app->getEventDispatcher()->listen("application.boot",[&](IEventPublisher* publisher){
        for(auto itr = this->m_serviceProviders.begin();itr != this->m_serviceProviders.end();++itr){
            (*itr)->boot();
            delete *itr;
        }
    },this->m_app);

    //  Finally delete the provider repository itself
    delete providerRepo;
}

/**
 * @brief Registers a single service provider and its service
 * 
 * @param provider The service provider to be registered
 */
void ServiceProviderManager::registerProvider(ServiceProvider* provider){
	g_message("ServiceProviderManager: Registering ServiceProvider that provides \"%s\"",provider->provides().c_str());
    //  Checking if the provider is previously registered
	if(isProviderRegistered(provider)){
        return;
    }
    //  Register the service provided by the service provider to the container
    provider->registerService();

    //  Finally marking the service provider as registered itself
    this->markAsRegistered(provider);
}

/**
 * @brief Checks if the provided provider is previously registered
 * 
 * @param provider The provider to be checked
 * @return bool
 * 
 * @todo Make this function const safe with const refrence parameter
 */
bool ServiceProviderManager::isProviderRegistered(ServiceProvider* provider){
	return (find(this->m_serviceProviders.begin(),this->m_serviceProviders.end(),provider) != this->m_serviceProviders.end());
}

/**
 * @brief Marks a provider as registered
 * 
 * @param provider The provider to be marked as registered
 */
void ServiceProviderManager::markAsRegistered(ServiceProvider* provider){
	this->m_serviceProviders.push_back(provider);
}

}
