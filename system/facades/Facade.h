#ifndef _FACADE_H_
#define _FACADE_H_

#include <string>
#include <system/foundation/Application.h>

using namespace std;

namespace tuber{

class IBindable;

class Facade{
public:
	static void setFacadeApplication(Application* app);
	static Application* getApp();
protected:
	static Application* m_app;
	static string getFacadeAccessor();
	static IBindable* get();
};

}

#endif
