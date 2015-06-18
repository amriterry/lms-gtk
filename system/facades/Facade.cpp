#include <system/facades/Facade.h>
#include <system/foundation/Application.h>
#include <system/foundation/RunTimeException.h>

namespace tuber{

/**
 * @brief Application object's pointer
 */
Application* Facade::m_app = nullptr;

/**
 * @brief Sets up the Application object into the Facade so that we can resolve objects out of it
 * 
 * @param app Application object to be set
 */
void Facade::setFacadeApplication(Application* app){
	m_app = app;
}

/**
 * @brief Returns the Application object
 * @return Application object
 */
Application* Facade::getApp(){
	return m_app;
}

/**
 * @brief Returns the name of object that the facade represents
 * @return string of bindingKey of object the facade represents
 */
string Facade::getFacadeAccessor(){
	throw RunTimeException("Facade does not implement getFacadeAccessor method.");
}

/**
 * @brief Returns the object that the facade represents
 * @return IBindable object resolved out of the container
 */
IBindable* Facade::get(){
	return m_app->make(Facade::getFacadeAccessor());
}


}