#include <gtk/gtk.h>

#include <system/database/DatabaseServiceProvider.h>

#include <system/foundation/Container.h>

#include <system/config/ConfigManager.h>
#include <system/database/connectors/ConnectionFactory.h>
#include <system/database/DatabaseManager.h>
#include <system/facades/facades.h>

#include <system/database/model/Model.h>
#include <system/database/Binding.h>

namespace tuber{

void DatabaseServiceProvider::boot(){
	g_message("DatabaseServiceProvider: Setting boot callback");
	Model::setConnectionResolver(DB::get());
	Model::setEventDispatcher(EventFacade::get());
}

void DatabaseServiceProvider::registerService(){
	this->m_container->bind("db.query.binding",[&](Container* container) -> IBindable*{
		return new Binding();
	});

	this->m_container->bindShared("db.factory",[&](Container* container) -> IBindable*{
		return new ConnectionFactory();
	});

	this->m_container->bindShared("db",[&](Container* container) -> IBindable*{
		return new DatabaseManager(container->resolve<ConfigManager>("ConfigManager"),container->resolve<ConnectionFactory>("db.factory"));
	});
}

string DatabaseServiceProvider::provides(){
	return "DatabaseManager";
}

}
