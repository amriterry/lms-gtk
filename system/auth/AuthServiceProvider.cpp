#include "AuthServiceProvider.h"

#include "AuthManager.h"

#include <system/foundation/Container.h>

namespace tuber{

void AuthServiceProvider::registerService(){
	this->m_container->bindShared("AuthManager",[&](Container* container) -> IBindable*{
		return new AuthManager();
	});
}

string AuthServiceProvider::provides(){
	return "AuthManager";
}

}
