#include <system/foundation/KeyNotFoundException.h>

namespace tuber{

const char* KeyNotFoundException::what() const throw(){
	string message = "Unexpected key provided: " + this->m_key;
	return message.c_str();
}

}
