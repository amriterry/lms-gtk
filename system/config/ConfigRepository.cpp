#include <fstream>

#include <iostream>
#include <regex>
#include <algorithm>

#include <gtk/gtk.h>

#include <system/config/ConfigRepository.h>

using namespace std;

namespace tuber{

ConfigRepository::ConfigRepository(string configFile) : m_configFile(configFile){
	ifstream file("config/" + configFile);
	string data;
	string line;

	if(file.is_open()){
        while(getline(file,line)){
            line.erase(std::remove_if(line.begin(),line.end(),[](char c){
                return (c == '\r' || c == '\t' || c== ' ' || c == '\n');
            }),line.end());
            data += line;
        }
        g_message("ConfigRepository: >>\n\n %s \n\n<<",data.c_str());

        this->m_document.Parse<0>(data.c_str());
        file.close();
	} else {
        g_error("Unable to open configuration file: config/%s",configFile.c_str());
	}
}

Document* ConfigRepository::getConfigDocument(){
    return &(this->m_document);
}

ConfigRepository::~ConfigRepository(){
    g_message("ConfigRepository: Deleting ConfigRepository");
}


}
