#include <system/request/ValidationServiceProvider.h>
#include <system/validation/ValidationFactory.h>
#include <system/foundation/Container.h>

namespace tuber{

void ValidationServiceProvider::registerService(){
	this->m_container->bindShared("ValidationFactory",[&](Container* container) -> IBindable*{
		return new ValidationFactory();
	});
}

string ValidationServiceProvider::provides(){
	return "ValidationFactory";
}

}
