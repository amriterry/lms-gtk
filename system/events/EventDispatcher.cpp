#include <map>
#include <vector>
#include <string>
#include <algorithm>

#include <gtk/gtk.h>

#include <system/events/EventDispatcher.h>
#include <system/support/IEventPublisher.h>
#include <system/bundle/Bundle.h>
#include <system/events/UnregisteredEventFiringException.h>

using namespace std;

namespace tuber{


//map<string,map<IEventPublisher*,vector<EventCallback>>> listeners;

void EventDispatcher::listen(string event,EventCallback callback,IEventPublisher* publisher){
	//this->listeners[event].insert(pair<IEventPublisher*,vector<EventCallback>(publisher,callback));
	if(isListening(event)){
        if(hasPublisher(event,publisher)){
            this->listeners[event].at(publisher).push_back(callback);
        } else {
            this->listeners[event].insert(pair<IEventPublisher*,vector<EventCallback>>(publisher,{callback}));
        }
	} else {
        this->listeners[event].insert(pair<IEventPublisher*,vector<EventCallback>>(publisher,{callback}));
	}

	g_message("EventDispatcher: Adding event >> %s",event.c_str());
}

void EventDispatcher::fire(string event){
    if(!isListening(event)){
        throw UnregisteredEventFiringException(event);
        return;
    }

    auto eventMap = listeners.at(event);

    for(auto it = eventMap.begin();it != eventMap.end(); ++it){
    	IEventPublisher* publisher = it->first;
    	for(auto it_vector = it->second.begin();it_vector != it->second.end(); ++it_vector){
    		(*it_vector)(publisher);
    	}
    }
}

void EventDispatcher::fireIfExists(string event){
    if(!isListening(event)){
        return;
    }

    fire(event);
}

bool EventDispatcher::isListening(string event){
	return (listeners.find(event) != listeners.end())?true:false;
}

bool EventDispatcher::hasPublisher(string event,IEventPublisher* publisher){
	if(isListening(event)){
		auto eventMap = listeners.at(event);
		return (eventMap.find(publisher) != eventMap.end())?true:false;
	} else {
		return false;
	}
}

}
