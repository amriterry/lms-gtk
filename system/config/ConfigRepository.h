#ifndef _CONFIG_REPOSITORY_H_
#define _CONFIG_REPOSITORY_H_

#include <string>

#include <vendor/rapidjson/document.h>

using namespace std;
using namespace rapidjson;

namespace tuber{

class ConfigRepository{
public:
	ConfigRepository(string configFile);
	Document* getConfigDocument();
	virtual ~ConfigRepository();
protected:
    string m_configFile;
    Document m_document;
};

}

#endif // _CONFIG_REPOSITORY_H_
