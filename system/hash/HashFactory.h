#ifndef _HASH_FACTORY_H_
#define _HASH_FACTORY_H_

#include <string>

#include <system/foundation/Service.h>

using namespace std;

namespace tuber{

class HashFactory: public Service{
public:
	HashFactory(string encryptionKey);
	string encrypt(string stringToCrypt);
	string decrypt(string stringToCrypt);
private:
	string m_encryptionKey;
};

}

#endif
