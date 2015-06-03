#ifndef _EVENT_FACADE_H_
#define _EVENT_FACADE_H_

#include <system/facades/Facade.h>

#include <system/events/EventDispatcher.h>

namespace tuber{

class EventFacade: public Facade{
public:
	static EventDispatcher* get(){
		return m_app->resolve<EventDispatcher>(getFacadeAccessor());
	}

	static string getFacadeAccessor(){
        return "EventDispatcher";
	}
};

}



#endif
