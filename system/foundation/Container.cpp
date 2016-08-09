#include <cstdlib>
#include <ctime>
#include <algorithm>

#include <gtk/gtk.h>

#include <system/foundation/Container.h>
#include <system/foundation/UnboundResolvingException.h>

namespace tuber{

/**
 * @brief Container 'ctor'
 */
Container::Container(){
    //  Seeding the random generator to the current time
    srand(time(NULL));
}

/**
 * @brief Container destructor
 */
Container::~Container(){
    //  Deleting all instances in the container
    for_each(instances.begin(),instances.end(),[&](pair<int,IBindable*> binding){
        delete binding.second;
    });
}

/**
 * @brief Checking if an object is bound to the container
 * 
 * @param bindingKey Key of the object to check for the bound
 * @return bool
 */
bool Container::isBound(string bindingKey){
    return (this->bindings.find(bindingKey) != this->bindings.end());
}

/**
 * @brief Checks if an object of a key is resolved
 * 
 * @param bindingKey Key for which the object resolved is to check
 * @return bool
 */
bool Container::isResolved(string bindingKey){
    return (this->resolved.find(bindingKey) != this->resolved.end());
}

/**
 * @brief Binds a resolving callback to the container with key
 * 
 * @param bindingKey Key with which the object can be later resolved
 * @param closure The resolving callback to generate the resolvable(IBindable) object
 * @param shared The boolean value to check if the object is shared across the application
 */
void Container::bind(string bindingKey,ResolvingCallback closure,bool shared){
    //  If any other object is not bound with the same key then
    //  Inserting callbacks and shared value to maps
    if(!this->isBound(bindingKey)){
        this->bindings[bindingKey] = shared;

        this->resolvingCallbacks[bindingKey] = closure;
    }
}

/**
 * @brief Binds shared object to the container
 * 
 * @param bindingKey Key with which the object can be later resolved
 * @param closure The resolving callback to generate the resolvable(IBindable) object
 */
void Container::bindShared(string bindingKey,ResolvingCallback closure){
    this->bind(bindingKey,closure,true);
}

/**
 * @brief Making the object from the binding key
 * 
 * @param bindingKey The key of object to be made
 * @return The made object from the key
 */
IBindable* Container::make(string bindingKey){
    IBindable* instance;
    if(this->isBound(bindingKey)){
        //  If the bindingKey is bounded, check if the binding is shared
        bool shared = this->bindings[bindingKey];

        if(shared){
            //  If the binded object is shared then check if the object is resolved
            if(this->isResolved(bindingKey)){
                // if the binded object is resolved then set the instance to it
                int resolvedId = this->resolved[bindingKey][0];
                instance = this->instances[resolvedId];
            } else {
                // if the object is not resolved the build it and set the instance to it
                instance = this->build(bindingKey);
            }
        } else {
            // if the object is not set to be shared then we build the object and set it to instance
            instance = this->build(bindingKey);
        }
    } else {
        //  If the bindingKey is not bounded throw a BindingException
        throw UnboundResolvingException(bindingKey);
        instance = NULL;
    }

    // if the instance is NULL, then throw the error and close the application
    if(instance == NULL){
        g_error("Not Enough Memory or trying to resolve unbound object");
    }

    return instance;
}

/**
 * @brief Builds the object from the key
 * 
 * @param bindingKey Key from which the object is to be created
 * @return The built IBindable object's pointer
 */
IBindable* Container::build(string bindingKey){
    int resolveId;
    IBindable* abstract;
    try{
        //  Trying to resolve abstract object using the resolving callback
        abstract = this->resolvingCallbacks[bindingKey](this);
    } catch (bad_function_call& e){
        g_error("%s",e.what());
    }

    //  Get unique resolvedId so that new object can be binded with it
    do{
        resolveId = rand() % rand();
    } while(hasResolveId(resolveId));

    //  Checking if any other object of the save key is resolved
    if(isResolved(bindingKey)){
        // if resolved push the resolvedId
        this->resolved[bindingKey].push_back(resolveId);
    } else {
        // if not resolved enter new pair into the resolved
        this->resolved.insert({bindingKey,{resolveId}});
    }
    // finally insret the instance with resolvedId to the instances
    this->instances.insert({resolveId,abstract});

    //  Finalizing the building process by setting identifier to the abstract object
    //  This makes the object itself hold the information about bindingKey and resolvedId
    abstract->setIdentifier(bindingKey,resolveId);

    return abstract;
}

/**
 * @brief Check if there is another same resolvedId
 * 
 * @param resolveId The resolvedId to be check for
 * @return bool
 */
bool Container::hasResolveId(int resolveId){
    return (instances.find(resolveId) != instances.end())?true:false;
}

/**
 * @brief Destroys the object binded to the container
 * 
 * @param bindableInstance Instance to be deleted
 * @return bool
 */
bool Container::destroyBindable(IBindable* bindableInstance){
    // Getting information about the object itself
    int resolveId = bindableInstance->getResolveId();
    string bindingKey = bindableInstance->getBindingKey();
    
    //  If the resolvedId does not exists then return false
    if(!hasResolveId(resolveId)){
        return false;
    }

    //  Deleting the instance itself and setting it to nullptr
    delete this->instances[resolveId];
    this->instances[resolveId] = nullptr;

    //  Erasing the instance information from the maps
    vector<int> resolvedIds;
    this->instances.erase(resolveId);
    resolvedIds = this->resolved[bindingKey];
    if(resolvedIds.size() == 1){
        this->resolved.erase(bindingKey);
    } else {
        int position = find(resolvedIds.begin(),resolvedIds.end(),resolveId) - resolvedIds.begin();
        resolvedIds.erase(resolvedIds.begin() + position);
    }
    return true;
}

}
