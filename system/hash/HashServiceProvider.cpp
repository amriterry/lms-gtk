#include <system/hash/HashServiceProvider.h>

#include <system/foundation/Application.h>
#include <system/hash/HashFactory.h>

namespace tuber{

void HashServiceProvider::registerService(){
	this->m_container->bindShared("HashFactory",[&](Container* container) -> IBindable*{
		return new HashFactory("key");
	});
}

string HashServiceProvider::provides(){
	return "HashFactory";
}

}
