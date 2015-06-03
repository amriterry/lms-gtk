#ifndef _CONFIG_MANAGER_H_
#define _CONFIG_MANAGER_H_

#include <map>
#include <string>

#include <vendor/rapidjson/document.h>

#include <system/support/IBindable.h>

using namespace std;

namespace tuber{

class Application;
class ConfigRepository;

class ConfigManager : public IBindable{
public:
	ConfigManager(Application* app);
	void loadDBConfiguration(rapidjson::Value* val);
	void loadControllerConfiguration(rapidjson::Value* val);
	void loadConfigDocument(rapidjson::Document* configDocument);

	string getDefaultController();
	string getDefaultDbDriver();
	map<string,string>& getDriverConfiguration(string driverKey);

	virtual ~ConfigManager();
protected:
    string m_defaultDriver;
    string m_defaultController;
    map<string,map<string,string>> m_dbDriverConfig;
    Application* m_app;
};

}

#endif //_CONFIG_MANAGER_H_
