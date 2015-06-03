#ifndef _UNREGISTERED_EVENT_FIRING_EXCEPTION_H_
#define _UNREGISTERED_EVENT_FIRING_EXCEPTION_H_

#include <exception>
#include <string>

using namespace std;

namespace tuber{

class UnregisteredEventFiringException : public exception{
public:
	UnregisteredEventFiringException(string event) : m_eventKey(event){}
	const char* what() const throw();
private:
	string m_eventKey;
};

}

#endif //_UNREGISTERED_EVENT_FIRING_EXCEPTION_H_
