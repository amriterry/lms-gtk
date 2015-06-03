#ifndef _CONFIG_PROVIDERS_H_
#define _CONFIG_PROVIDERS_H_

#include <gtk/gtk.h>

#include <vector>
#include <system/foundation/ProviderRepository.h>

using namespace std;
using namespace tuber;

class AppProviderRepository : public ProviderRepository{
public:
    AppProviderRepository(Application *app);
    virtual vector<ServiceProvider*> getProviders();
private:
    vector<ServiceProvider*> m_providers;
};

#endif // _CONFIG_PROVIDERS_H_
