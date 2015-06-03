#include <system/controller/ControllerStateManager.h>

namespace tuber{

ControllerStateManager::ControllerStateManager(){
	this->m_state = ControllerState::RESOLVE;
}

bool ControllerStateManager::isResolved(){
	return (this->m_state == ControllerState::RESOLVE)?true:false;
}

bool ControllerStateManager::isCreated(){
	return (this->m_state == ControllerState::CREATE)?true:false;
}

bool ControllerStateManager::isStarted(){
	return (this->m_state == ControllerState::START)?true:false;
}

bool ControllerStateManager::isResumed(){
	return (this->m_state == ControllerState::RESUME)?true:false;
}

bool ControllerStateManager::isPaused(){
	return (this->m_state == ControllerState::PAUSE)?true:false;
}

bool ControllerStateManager::isStopped(){
	return (this->m_state == ControllerState::STOP)?true:false;
}

void ControllerStateManager::setState(ControllerState state){
	this->m_state = state;
}

ControllerState ControllerStateManager::getState(){
	return this->m_state;
}

}
