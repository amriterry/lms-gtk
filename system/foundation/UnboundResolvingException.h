#ifndef _UNBOUND_RESOLVING_EXCEPTION_H_
#define _UNBOUND_RESOLVING_EXCEPTION_H_

#include <exception>
#include <string>

using namespace std;

namespace tuber{

class UnboundResolvingException: public exception{
public:
	UnboundResolvingException(string bindingKey) : m_bindingKey(bindingKey){}
	const char* what() const throw();
private:
	string m_bindingKey;
};

}

#endif //_UNBOUND_RESOLVING_EXCEPTION_H_
