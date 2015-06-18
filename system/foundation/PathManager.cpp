#include <system/foundation/PathManager.h>
#include <system/foundation/KeyNotFoundException.h>

namespace tuber{

/**
 * @brief Returns the path from the path key
 * 
 * @param pathKey The key of the path to be found
 * @return Path from the path key
 */
string PathManager::getPath(string pathKey){
	if(hasPath(pathKey)){
		return this->m_paths[pathKey];
	} else {
		throw KeyNotFoundException(pathKey);
	}
}

/**
 * @brief Checks if there is a path with the key
 * 
 * @param pathKey The path of the key to be found
 * @return bool
 */
bool PathManager::hasPath(string pathKey){
	if(this->m_paths.find(pathKey) != this->m_paths.end()){
		return true;
	} else {
		return false;
	}
}

/**
 * @brief Setting the path with key
 * 
 * @param pathKey Key with which the path is to be accessed
 * @param pathValue The path value
 * 
 * @return Returns the path manager object itself for chaining
 */
PathManager* PathManager::setPath(string pathKey,string pathValue){
	this->m_paths[pathKey] = pathValue;
	return this;
}

}
