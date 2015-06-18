#include <iostream>

#include <gtk/gtk.h>

#include <system/config/ConfigManager.h>
#include <config/AppProviderRepository.h>
#include <system/foundation/Application.h>
#include <system/config/ConfigRepository.h>
#include <controllers/AppControllerRepository.h>

using namespace std;
using namespace tuber;

int main(int argc,char** argv){
    //  For the applicaiton to run
    //  1.  Create Application
    //  2.  Load Configurations
    //  3.  Load ServiceProviders
    //  4.  Load Controllers
    //  5.  Setting up default controller
    //  6.  Run application

    //  Creates the application with 'tuber.lms' as application Identifier
    //  and 'Librar Management System' as application name
    Application* app = Application::create("tuber.lms","Library Management System");

    //  Loading configurations from the configrepository
    app->loadConfiguration(new ConfigRepository("app.json"));

    //  Loading ServiceProvders from the ProvderRepository
    app->loadProviders(new AppProviderRepository(app));

    //  Loading controllers from the ControllerRepository
    app->loadControllers(new AppControllerRepository(app));

    //  Setting up default controller to the ConfigManager
    app->getConfigManager()->setDefaultController("SplashController");

    //  Running the application
	return app->run(argc,argv);
}
