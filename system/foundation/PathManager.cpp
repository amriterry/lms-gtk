#include <system/foundation/PathManager.h>
#include <system/foundation/KeyNotFoundException.h>

namespace tuber{

string PathManager::getPath(string pathKey){
	if(hasPath(pathKey)){
		return this->m_paths[pathKey];
	} else {
		throw KeyNotFoundException(pathKey);
	}
}

bool PathManager::hasPath(string pathKey){
	if(this->m_paths.find(pathKey) != this->m_paths.end()){
		return true;
	} else {
		return false;
	}
}

PathManager* PathManager::setPath(string pathKey,string pathValue){
	this->m_paths[pathKey] = pathValue;
	return this;
}

}
