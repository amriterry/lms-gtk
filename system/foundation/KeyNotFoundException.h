#ifndef _KEY_NOT_FOUND_EXCEPTION_H_
#define _KEY_NOT_FOUND_EXCEPTION_H_

#include <exception>
#include <string>

using namespace std;

namespace tuber{

class KeyNotFoundException: public exception{
public:
	KeyNotFoundException(string key) : m_key(key){}
	const char* what() const throw();
private:
	string m_key;
};

}

#endif