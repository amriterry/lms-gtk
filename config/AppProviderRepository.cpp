#include <vector>

#include <config/AppProviderRepository.h>

#include <system/bundle/BundleServiceProvider.h>
#include <system/request/RequestServiceProvider.h>
#include <system/test/TestServiceProvider.h>
#include <system/hash/HashServiceProvider.h>
#include <system/database/DatabaseServiceProvider.h>
#include <system/auth/AuthServiceProvider.h>

using namespace std;
using namespace tuber;

AppProviderRepository::AppProviderRepository(Application *app) : ProviderRepository(app){
	this->m_providers = {
        new BundleServiceProvider(app),
        new RequestServiceProvider(app),
        new HashServiceProvider(app),
        new DatabaseServiceProvider(app),
        new AuthServiceProvider(app)
	};
}

vector<ServiceProvider*> AppProviderRepository::getProviders(){
    return this->m_providers;
}


