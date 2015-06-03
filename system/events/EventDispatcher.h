#ifndef _EVENT_DISPATCHER_H_
#define _EVENT_DISPATCHER_H_

#include <map>
#include <vector>
#include <string>
#include <functional>

#include <gtk/gtk.h>

#include <system/foundation/Service.h>
#include <system/support/IEventPublisher.h>

using namespace std;

namespace tuber{

class Bundle;

typedef void (*g_signal_callback)(GtkWidget*,gpointer);
typedef function<void(IEventPublisher*)> EventCallback;

//typedef void (*EventCallback)(IEventPublisher*);

class EventDispatcher : public Service{
public:
    EventDispatcher(){ g_message("EventDispatcher: created"); }
	void listen(string event,EventCallback callback,IEventPublisher* publisher);
	void fire(string event);
	void fireIfExists(string event);

	~EventDispatcher() { g_message("Event Dispatcher deleting"); }
private:
	map<string,map<IEventPublisher*,vector<EventCallback>>> listeners;
	bool isListening(string event);
	bool hasPublisher(string event,IEventPublisher* publisher);
};

}

#endif //_EVENT_DISPATCHER_H_
