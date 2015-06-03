#include <system/foundation/RunTimeException.h>

namespace tuber{

const char* RunTimeException::what() const throw(){
	return this->m_message.c_str();
}

}
