#ifndef _SERVICE_H_
#define _SERVICE_H_

#include <system/support/IBindable.h>

namespace tuber{

class Service : public IBindable{
protected:
	Service(){}
	virtual ~Service(){}
};

}

#endif //_SERVICE_H_
