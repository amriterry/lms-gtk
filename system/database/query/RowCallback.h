#ifndef _ROW_CALLBACK_H_
#define _ROW_CALLBACK_H_

#include <functional>

using namespace std;

namespace tuber{

class Statement;
class Bundle;

typedef function<void(Statement*,Bundle*)> RowCallback;

}

#endif
