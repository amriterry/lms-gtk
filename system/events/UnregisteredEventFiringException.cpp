#include <system/events/UnregisteredEventFiringException.h>

using namespace std;

namespace tuber{

const char* UnregisteredEventFiringException::what() const throw(){
	string message = "Trying to fire unregistered event: \"" + this->m_eventKey + "\"";
	return message.c_str();
}

}
