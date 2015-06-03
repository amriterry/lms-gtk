#include <iostream>

#include <gtk/gtk.h>

#include <system/foundation/Application.h>
#include <system/config/ConfigRepository.h>
#include <config/AppProviderRepository.h>
#include <controllers/AppControllerRepository.h>


using namespace std;
using namespace tuber;

int main(int argc,char** argv){
    Application *app = Application::create("tuber.lms","Library Management System");
    //  1.  Create Application
    //  2.  Load Configurations
    //  3.  Load ServiceProviders
    app->loadConfiguration(new ConfigRepository("app.json"));

    app->loadProviders(new AppProviderRepository(app));

    app->loadControllers(new AppControllerRepository(app));

	return app->run(argc,argv);
}
