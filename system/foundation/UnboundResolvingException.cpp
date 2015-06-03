#include <system/foundation/UnboundResolvingException.h>

namespace tuber{

const char* UnboundResolvingException::what() const throw(){
	string message = "Trying to resolve unbounded object out of Container with key: \"" + this->m_bindingKey + "\"";
	return message.c_str();
}

}
