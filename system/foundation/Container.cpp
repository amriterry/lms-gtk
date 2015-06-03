#include <cstdlib>
#include <ctime>
#include <algorithm>

#include <gtk/gtk.h>

#include <system/foundation/Container.h>
#include <system/foundation/UnboundResolvingException.h>

namespace tuber{

Container::Container(){
    srand(time(NULL));
}

bool Container::isBound(string bindingKey){
    return (this->bindings.find(bindingKey) != this->bindings.end());
}

bool Container::isResolved(string bindingKey){
    return (this->resolved.find(bindingKey) != this->resolved.end());
}

void Container::bind(string bindingKey,ResolvingCallback closure,bool shared){
    if(!this->isBound(bindingKey)){
        this->bindings[bindingKey] = shared;

        this->resolvingCallbacks[bindingKey] = closure;
        g_message("Container: Binding callback for key:[%s]",bindingKey.c_str());
    }
}

void Container::bindShared(string bindingKey,ResolvingCallback closure){
    this->bind(bindingKey,closure,true);
}

IBindable* Container::make(string bindingKey){
    IBindable* instance;
    if(this->isBound(bindingKey)){
        //If the bindingKey is bounded, check if the binding is shared
        bool shared = this->bindings[bindingKey];

        if(shared){
            if(this->isResolved(bindingKey)){
                int resolvedId = this->resolved[bindingKey][0];
                instance = this->instances[resolvedId];
            } else {
                instance = this->build(bindingKey);
            }
        } else {
            instance = this->build(bindingKey);
        }
    } else {
        //If the bindingKey is not bounded throw a BindingException
        throw UnboundResolvingException(bindingKey);
        instance = NULL;
    }

    if(instance == NULL){
        g_error("Not Enough Memory or trying to resolve unbound object");
    }

    g_message("Container: made instance of %s",bindingKey.c_str());

    return instance;
}

IBindable* Container::build(string bindingKey){
    int resolveId;
    IBindable* abstract;
    try{
        abstract = this->resolvingCallbacks[bindingKey](this);
    } catch (bad_function_call& e){
        g_message("%s",e.what());
    }

    do{
        resolveId = rand() % rand();
    } while(hasResolveId(resolveId));

    if(isResolved(bindingKey)){
        this->resolved[bindingKey].push_back(resolveId);
    } else {
        this->resolved.insert({bindingKey,{resolveId}});
    }
    this->instances.insert({resolveId,abstract});

    g_message("Container: Built instance of %s",bindingKey.c_str());

    abstract->setIdentifier(bindingKey,resolveId);

    return abstract;
}

Container::~Container(){
    g_message("Container: Deleting Instances");
    for_each(instances.begin(),instances.end(),[&](pair<int,IBindable*> binding){
        delete binding.second;
    });
}

bool Container::hasResolveId(int resolveId){
    return (instances.find(resolveId) != instances.end())?true:false;
}

bool Container::destroyBindable(IBindable* bindableInstance){
    int resolveId = bindableInstance->getResolveId();
    string bindingKey = bindableInstance->getBindingKey();
    vector<int> resolvedIds;

    delete this->instances[resolveId];

    this->instances.erase(resolveId);
    resolvedIds = this->resolved[bindingKey];
    if(resolvedIds.size() == 1){
        this->resolved.erase(bindingKey);
    } else {
        int position = find(resolvedIds.begin(),resolvedIds.end(),resolveId) - resolvedIds.begin();
        resolvedIds.erase(resolvedIds.begin() + position);
    }
}

}
