#ifndef _REQUEST_SERVICE_PROVIDER_H_
#define _REQUEST_SERVICE_PROVIDER_H_

#include <string>

#include <gtk/gtk.h>

#include <system/foundation/ServiceProvider.h>

using namespace std;

namespace tuber{

class RequestServiceProvider: public ServiceProvider{
public:
	RequestServiceProvider(Container* container) : ServiceProvider(container){
		g_message("RequestServiceProvider: created RequestServiceProvider");
	}
	void registerService();
	string provides();
	~RequestServiceProvider(){
		g_message("RequestServiceProvider: RequestServiceProvider object deleting");
	}
};

}
#endif
