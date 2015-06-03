#ifndef _CONTAINER_H_
#define _CONTAINER_H_

#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <functional>
#include <gtk/gtk.h>
#include <system/support/IBindable.h>

using namespace std;

namespace tuber{

class Container;

typedef function<IBindable*(Container*)> ResolvingCallback;

class Container{
public:
    void bind(string bindingKey,ResolvingCallback closure,bool shared = false);
    void bindShared(string bindingKey,ResolvingCallback closure);
    IBindable* make(string bindingKey);
    IBindable* build(string bindingKey);

    //Moving implementation to header file
    template<typename T>
    T* resolve(string bindingKey){
        IBindable* instance = this->make(bindingKey);
        if(instance != NULL){
            return (T*)(instance);
        } else {
            return NULL;
        }
    }

    bool isBound(string bindingKey);
    bool isResolved(string bindingKey);
    bool destroyBindable(IBindable* bindableInstance);
protected:
    Container();
    virtual ~Container();

    map<string,bool> bindings;
    map<string, vector<int>> resolved;
    unordered_map<int, IBindable*> instances;
    map<string, ResolvingCallback> resolvingCallbacks;
private:
    bool hasResolveId(int resolveId);
};

}

#endif //_CONTAINER_H_
