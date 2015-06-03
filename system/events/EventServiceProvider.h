#ifndef _EVENT_SERVICE_PROVIDER_H_
#define _EVENT_SERVICE_PROVIDER_H_

#include <system/foundation/Container.h>
#include <system/foundation/ServiceProvider.h>
#include <system/events/EventDispatcher.h>

namespace tuber{

class EventServiceProvider : public ServiceProvider{
public:
    EventServiceProvider(Container* container) : ServiceProvider(container){
    	g_message("EventServiceProvider: Creating EventServiceProvider");
    }
    virtual void registerService();
    virtual string provides();
    ~EventServiceProvider();
};

}

#endif // _EVENT_SERVICE_PROVIDER_H_
