#ifndef _PATH_MANAGER_H_
#define _PATH_MANAGER_H_

#include <map>
#include <string>

#include <system/support/IBindable.h>

using namespace std;

namespace tuber{

class PathManager: public IBindable{
public:
	string getPath(string pathKey);
	bool hasPath(string pathKey);
	PathManager* setPath(string pathKey,string pathValue);	
protected:
	map<string,string> m_paths;
};

}

#endif