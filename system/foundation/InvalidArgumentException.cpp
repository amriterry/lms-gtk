#include <system/foundation/InvalidArgumentException.h>

namespace tuber{

const char* InvalidArgumentException::what() const throw(){
	return this->m_message.c_str();
}

}
