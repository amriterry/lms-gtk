#ifndef _CONTROLLER_STATE_MANAGER_H_
#define _CONTROLLER_STATE_MANAGER_H_

#include <system/support/IBindable.h>
#include <system/controller/ControllerState.h>

namespace tuber{

class ControllerStateManager: public IBindable{
public:
	ControllerStateManager();
	bool isResolved();
	bool isCreated();
	bool isStarted();
	bool isResumed();
	bool isPaused();
	bool isStopped();
	void setState(ControllerState state);
	ControllerState getState();
private:
	ControllerState m_state;
};

}

#endif //_CONTROLLER_STATE_MANAGER_H_
