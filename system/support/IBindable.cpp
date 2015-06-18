#include <system/support/IBindable.h>

#include <system/facades/Facades.h>

namespace tuber{

/**
 * @brief Setter for the bindable object's bindingKey and resolvedId
 * 
 * @param bindingKey string of bindingKey of the Bindable object
 * @param resolveId int of resolvedId of the Bindable object
 */
void IBindable::setIdentifier(string bindingKey,int resolveId){
	this->m_bindingKey = bindingKey;
	this->m_resolveId = resolveId;
}

/**
 * @brief Getter for the resolvedId of this object
 * @return The resolvedId used in container to store the refrence of the object
 * 
 * @todo Make this const safe
 */
int IBindable::getResolveId(){
	return this->m_resolveId;
}

/**
 * @brief Getter for the binding key of the object
 * @return The binding key used to store the object in contaner
 * 
 * @todo Make this method const safe
 */
string IBindable::getBindingKey(){
	return this->m_bindingKey;
}

/**
 * @brief Object deletes itself using the container
 */
void IBindable::commitSucide(){
	//...commit sucide
	App::destroy(this);
}

}
