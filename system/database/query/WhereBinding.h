#ifndef _WHERE_BINDING_H_
#define _WHERE_BINDING_H_

#include <string>

#include <system/bundle/BindingType.h>

using namespace std;

namespace tuber{

struct WhereBinding{
	int position;
	string column;
	string operatorValue;
	string boolean;
	BindingType type;
};

}

#endif