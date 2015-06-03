#ifndef _RUN_TIME_EXCEPTION_H_
#define _RUN_TIME_EXCEPTION_H_

#include <exception>
#include <string>

using namespace std;

namespace tuber{

class RunTimeException: public exception{
public:
	RunTimeException(string message) : m_message(message){}
	const char* what() const throw();
private:
	string m_message;
};

}

#endif //_RUN_TIME_EXCEPTION_H_
