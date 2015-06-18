#ifndef _CONFIG_MANAGER_H_
#define _CONFIG_MANAGER_H_

#include <map>
#include <string>

#include <vendor/rapidjson/document.h>

#include <system/support/IBindable.h>
#include <system/support/IEventPublisher.h>

using namespace std;

namespace tuber{

class Application;
class ConfigRepository;

class ConfigManager : public IBindable, public IEventPublisher{
public:
	ConfigManager(Application* app);
	void loadDBConfiguration(rapidjson::Value* val);
	void loadConfigDocument(rapidjson::Document* configDocument);
	void setDefaultController(string controllerKey);

	string getDefaultController();
	string getDefaultDbConnection();
	map<string,string>& getConnectionConfiguration(string connectionKey);

	virtual ~ConfigManager();
protected:
    string m_defaultConnection;
    string m_defaultController;
    map<string,map<string,string>> m_connectionConfig;
    Application* m_app;
};

}

#endif //_CONFIG_MANAGER_H_
