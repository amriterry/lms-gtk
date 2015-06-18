#include <gtk/gtk.h>
#include <map>

#include <system/config/ConfigManager.h>
#include <system/foundation/Application.h>
#include <system/events/EventDispatcher.h>

using namespace std;

namespace tuber{

/**
 * @brief ConfigManager 'ctor'
 * 
 * @param app Application object
 */
ConfigManager::ConfigManager(Application* app) : m_app(app){

}

/**
 * @brief Loads config document and loads individual configurations
 * 
 * @param configDocument Document to be loaded
 */
void ConfigManager::loadConfigDocument(rapidjson::Document* configDocument){
    //  checking if the document has a member config which must contain the configurations
    if(!(configDocument->HasMember("config") && (*configDocument)["config"].IsObject())){
        g_error("Unable to load configuration. Check 'app.json' file for details");
    }

    //  Loading config into a Value object
    rapidjson::Value* mainConfig = &((*configDocument)["config"]);

    //  checks if there is database configuration
    //  if exists load database configuration
    //  else throw an error
    //  @todo   Replace error with something so that application which
    //          donot use databae should not include the database configuration
    if(mainConfig->HasMember("database") && (*mainConfig).IsObject()){
        this->loadDBConfiguration(&((*mainConfig)["database"]));
    } else {
        g_error("Unable to Load database configuration. Check 'app.json' for details");
    }

    //  After all the config is loaded
    //  Fire 'application.config.loaded' event
    this->m_app->getEventDispatcher()->fire("application.config.loaded");
}

/**
 * @brief Loads the database configuration
 * 
 * @param val Value object containing database configuration
 */
void ConfigManager::loadDBConfiguration(rapidjson::Value* val){
    //  Getting list of connections
    rapidjson::Value* connections = &((*val)["connections"]);

    if(!connections->IsObject()){
        g_error("Unable to Load Database driver configuration. Check 'app.json' for details");
    } else {
        //  Looping through each connection configuration and storing them in the object
        for(rapidjson::Value::ConstMemberIterator itr = connections->MemberBegin();itr != connections->MemberEnd(); ++itr){
            map<string,string> driverConfig;
            for(rapidjson::Value::ConstMemberIterator itConfig = itr->value.MemberBegin();itConfig != itr->value.MemberEnd(); ++itConfig){
                if(itr->name.IsString() && itConfig->name.IsString() && itConfig->value.IsString()){
                    driverConfig[itConfig->name.GetString()] = itConfig->value.GetString();
                } else {
                    g_error("Unable to Load Database driver configuration. Check 'app.json' for details");
                }
            }

            this->m_connectionConfig.insert({itr->name.GetString(),driverConfig});
        }
    }

    //  Getting default connection key from the value object
    if((*val)["default"].IsString()){
        this->m_defaultConnection = (*val)["default"].GetString();
        g_message("ConfigManager: Default Connection : [%s]",m_defaultConnection.c_str());
    } else {
        g_error("Unable to Load Database driver configuration. Check 'app.json' for details");
    }
}

/**
 * @brief Sets up the default controller key
 * 
 * @param controllerKey Default key string
 */
void ConfigManager::setDefaultController(string controllerKey){
    this->m_defaultController = controllerKey;
}

/**
 * @brief Getter for the default controller key
 * @return String key for default controller
 */
string ConfigManager::getDefaultController(){
    return this->m_defaultController;
}

/**
 * @brief Gettter for default connection key
 * @return String key for default connection
 */
string ConfigManager::getDefaultDbConnection(){
    return this->m_defaultConnection;;
}

/**
 * @brief Getter for connection configuration for the given key
 * 
 * @param connectionKey connection key for which the configuration is to be found
 * @return Connection configuration map
 */
map<string,string>& ConfigManager::getConnectionConfiguration(string connectionKey){
    return this->m_connectionConfig[connectionKey];
}

/**
 * @brief ConfigManager destructor
 */
ConfigManager::~ConfigManager(){

}

}
