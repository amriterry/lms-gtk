#ifndef _CONNECTOR_H_
#define _CONNECTOR_H_

#include <map>
#include <string>

using namespace std;

namespace tuber{

class Connector{
public:
	virtual void* createRawConnection(map<string,string> config) = 0;
};

}

#endif
