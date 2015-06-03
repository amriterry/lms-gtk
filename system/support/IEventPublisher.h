#ifndef _I_EVENT_PUBLISHER_H_
#define _I_EVENT_PUBLISHER_H_

namespace tuber{

class IEventPublisher{
public:
    IEventPublisher(){}
    virtual ~IEventPublisher(){}
	//virtual IEventPublisher* getPublisher() = 0;
};

}

#endif //_I_EVENT_PUBLISHER_H_
