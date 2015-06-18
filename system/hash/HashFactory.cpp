#include <gtk/gtk.h>

#include <system/hash/HashFactory.h>

#include <vendor/hashlib/hashlibpp.h>

namespace tuber{

HashFactory::HashFactory(string encryptionKey) : m_encryptionKey(encryptionKey){
	this->m_hashWrapper = new md5wrapper();
}

string HashFactory::encrypt(string stringToEncrypt){
    try{
        string hashed = this->m_hashWrapper->getHashFromString(stringToEncrypt);

        return hashed;
    } catch(hlException &e) {
    	g_error("%s",e.error_message().c_str());
    }
}

HashFactory::~HashFactory(){
	delete this->m_hashWrapper;
}

}
