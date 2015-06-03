#include <system/hash/HashFactory.h>

namespace tuber{

HashFactory::HashFactory(string encryptionKey) : m_encryptionKey(encryptionKey){}

string HashFactory::encrypt(string stringToEncrypt){
	if(!this->m_encryptionKey.size()){
		return stringToEncrypt;
	}

	for(string::size_type i = 0;i < stringToEncrypt.size(); ++i){
		stringToEncrypt[i] ^= m_encryptionKey[i%this->m_encryptionKey.size()];
	}

	return stringToEncrypt;
}

string HashFactory::decrypt(string stringToDecrypt){
	return this->encrypt(stringToDecrypt);
}

}
