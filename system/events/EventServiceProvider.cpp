#include <gtk/gtk.h>

#include <system/events/EventServiceProvider.h>
#include <system/events/EventDispatcher.h>

namespace tuber{

void EventServiceProvider::registerService(){
    this->m_container->bind("EventDispatcher",[](Container* container) -> IBindable*{
        return new EventDispatcher();
    },true);
}

string EventServiceProvider::provides(){
	return "EventDispatcher";
}

EventServiceProvider::~EventServiceProvider(){
    g_message("EventServiceProvider: Deleting EventServiceProvider");
}

}
