#include <system/request/RequestServiceProvider.h>
#include <system/request/Request.h>
#include <system/foundation/Container.h>

namespace tuber{

void RequestServiceProvider::registerService(){
	this->m_container->bind("Request",[&](Container* container) -> IBindable*{
		return new Request();
	});
}

string RequestServiceProvider::provides(){
	return "Request";
}

}
