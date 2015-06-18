#include <gtk/gtk.h>

#include <system/bundle/BundleServiceProvider.h>
#include <system/bundle/Bundle.h>

namespace tuber{

void BundleServiceProvider::boot(){
    g_message("BundleServiceProvider: Application booted so this function is running");
}

void BundleServiceProvider::registerService(){
	this->m_container->bind("Bundle",[&](Container* container) -> IBindable*{
		return new Bundle();
	});
}

string BundleServiceProvider::provides(){
	return "Bundle";
}

BundleServiceProvider::~BundleServiceProvider(){
	g_message("BundleServiceProvider: deleting BundleServiceProvider");
}

}
