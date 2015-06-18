#ifndef _HASH_FACTORY_H_
#define _HASH_FACTORY_H_

#include <string>

#include <system/foundation/Service.h>

using namespace std;

class hashwrapper;

namespace tuber{

class HashFactory: public Service{
public:
	HashFactory(string encryptionKey);
	string encrypt(string stringToCrypt);
	virtual ~HashFactory();
private:
	string m_encryptionKey;
	hashwrapper* m_hashWrapper;
};

}

#endif
