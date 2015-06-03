#ifndef _TEST_SERVICE_PROVIDER_H_
#define _TEST_SERVICE_PROVIDER_H_

#include <gtk/gtk.h>

#include <system/foundation/Service.h>
#include <system/foundation/ServiceProvider.h>
#include <system/foundation/Container.h>

using namespace tuber;

class Foo: public Service{
public:
    Foo(){
        g_message("Creating Foo");
    }
    ~Foo(){
        g_message("Deleting Foo");
    }
};

class Bar: public Service{
public:
    Bar(){
        g_message("Creating Bar");
    }
    ~Bar(){
        g_message("Deleting Bar");
    }
};

class FooServiceProvider: public ServiceProvider{
public:
    FooServiceProvider(Container* container) : ServiceProvider(container){}
    virtual void registerService(){
        g_message("Inside registerService of FooServiceProvider");
        this->m_container->bind("Foo",[](Container* container) -> IBindable*{
            g_message("Calling bind method");
            return new Foo();
        },false);
    }
    ~FooServiceProvider(){
        g_message("Deleting FooServiceProvider");
    }
};

class BarServiceProvider: public ServiceProvider{
public:
    BarServiceProvider(Container* container) : ServiceProvider(container){}
    virtual void registerService(){
        g_message("Inside registerService of BarServiceProvider");
        this->m_container->bind("Bar",[](Container* container) -> IBindable*{
            g_message("Calling bind method");
            return new Bar();
        },false);
    }
    ~BarServiceProvider(){
        g_message("Deleting BarServiceProvider");
    }
};
#endif // _TEST_SERVICE_PROVIDER_H_
