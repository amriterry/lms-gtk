#ifndef _PROVIDER_REPOSITORY_H_
#define _PROVIDER_REPOSITORY_H_

#include <vector>
#include <system/foundation/Application.h>

using namespace std;

namespace tuber{

class ServiceProvider;

class ProviderRepository{
public:
    virtual ~ProviderRepository(){}
    virtual vector<ServiceProvider*> getProviders() = 0;
protected:
    ProviderRepository(Application *app) : m_app(app){}
    Application* m_app;
};

}

#endif // _PROVIDER_REPOSITORY_H_
