#include <system/views/ViewServiceProvider.h>

#include <system/foundation/Application.h>
#include <system/views/ViewManager.h>

namespace tuber{

void ViewServiceProvider::registerService(){
	this->m_container->bindShared("ViewManager",[&](Container* container) -> IBindable*{
		return new ViewManager(static_cast<Application*>(container));
	});
}

string ViewServiceProvider::provides(){
	return "ViewManager";
}

}
