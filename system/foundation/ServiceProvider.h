#ifndef _SERVICE_PROVIDER_H_
#define _SERVICE_PROVIDER_H_

#include <string>

using namespace std;

namespace tuber{

class Container;

class ServiceProvider{
public:
    virtual void boot(){}
    virtual string provides(){return "";}
    virtual ~ServiceProvider(){}
    virtual void registerService() = 0;
protected:
    ServiceProvider(Container *container) : m_container(container){}
    Container* m_container;
};

}

#endif // _SERVICE_PROVIDER_H_
