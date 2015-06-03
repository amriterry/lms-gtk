#include <gtk/gtk.h>
#include <map>

#include <system/config/ConfigManager.h>

using namespace std;

namespace tuber{

ConfigManager::ConfigManager(Application* app) : m_app(app){

}

void ConfigManager::loadConfigDocument(rapidjson::Document* configDocument){
    if(!(configDocument->HasMember("config") && (*configDocument)["config"].IsObject())){
        g_error("Unable to load configuration. Check 'app.json' file for details");
    }

    rapidjson::Value* mainConfig = &((*configDocument)["config"]);

    if(mainConfig->HasMember("database") && (*mainConfig).IsObject()){
        this->loadDBConfiguration(&((*mainConfig)["database"]));
    } else {
        g_error("Unable to Load database configuration. Check 'app.json' for details");
    }

    if(mainConfig->HasMember("defaultController") && (*mainConfig)["defaultController"].IsString()){
        this->loadControllerConfiguration(&((*mainConfig)["defaultController"]));
    } else {
        g_error("Unable to Load controller configuration. Check 'app.json' for details");
    }
}

void ConfigManager::loadDBConfiguration(rapidjson::Value* val){
    rapidjson::Value* drivers = &((*val)["drivers"]);

    if(!drivers->IsObject()){
        g_error("Unable to Load Database driver configuration. Check 'app.json' for details");
    } else {
        for(rapidjson::Value::ConstMemberIterator itr = drivers->MemberBegin();itr != drivers->MemberEnd(); ++itr){
            map<string,string> driverConfig;
            for(rapidjson::Value::ConstMemberIterator itConfig = itr->value.MemberBegin();itConfig != itr->value.MemberEnd(); ++itConfig){
                if(itr->name.IsString() && itConfig->name.IsString() && itConfig->value.IsString()){
                    driverConfig[itConfig->name.GetString()] = itConfig->value.GetString();
                } else {
                    g_error("Unable to Load Database driver configuration. Check 'app.json' for details");
                }
            }

            this->m_dbDriverConfig.insert({itr->name.GetString(),driverConfig});
        }
    }

    if((*val)["defaultDriver"].IsString()){
        this->m_defaultDriver = (*val)["defaultDriver"].GetString();
    } else {
        g_error("Unable to Load Database driver configuration. Check 'app.json' for details");
    }
}

void ConfigManager::loadControllerConfiguration(rapidjson::Value* val){
    this->m_defaultController = val->GetString();
    g_message("ConfigManager: Default Controller[%s]",m_defaultController.c_str());
}

string ConfigManager::getDefaultController(){
    return this->m_defaultController;
}

string ConfigManager::getDefaultDbDriver(){
    return this->m_defaultDriver;
}

map<string,string>& ConfigManager::getDriverConfiguration(string driverKey){
    return this->m_dbDriverConfig[driverKey];
}

ConfigManager::~ConfigManager(){

}

}
