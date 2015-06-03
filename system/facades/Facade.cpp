#include <system/facades/Facade.h>
#include <system/foundation/Application.h>
#include <system/foundation/RunTimeException.h>

namespace tuber{

Application* Facade::m_app = nullptr;

void Facade::setFacadeApplication(Application* app){
	m_app = app;
}
Application* Facade::getApp(){
	return m_app;
}

string Facade::getFacadeAccessor(){
	throw RunTimeException("Facade does not implement getFacadeAccessor method.");
}

IBindable* Facade::get(){
	return m_app->make(Facade::getFacadeAccessor());
}


}