#ifndef _INVALID_ARGUMENT_EXCEPTION_H_
#define _INVALID_ARGUMENT_EXCEPTION_H_

#include <exception>
#include <string>

using namespace std;

namespace tuber{

class InvalidArgumentException: public exception{
public:
	InvalidArgumentException(string message) : m_message(message){}
	const char* what() const throw();
private:
	string m_message;
};

}

#endif