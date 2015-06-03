#ifndef _PATH_FACADE_H_
#define _PATH_FACADE_H_

#include <system/facades/Facade.h>

#include <system/foundation/PathManager.h>

namespace tuber{

class PathFacade: public Facade{
public:
	static PathManager* get(){
		return m_app->resolve<PathManager>(getFacadeAccessor());
	}

	static string getFacadeAccessor(){
        return "PathManager";
	}
};

}



#endif
