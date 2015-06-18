#ifndef _QUERY_RESULT_H_
#define _QUERY_RESULT_H_

#include <map>
#include <vector>
#include <string>

#include <system/bundle/Bundle.h>

using namespace std;

namespace tuber{

typedef Bundle* QueryRow;
typedef vector<QueryRow> QueryResult;

}


#endif