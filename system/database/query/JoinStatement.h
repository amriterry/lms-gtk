#ifndef _JOIN_STATEMENT_H_
#define _JOIN_STATEMENT_H_

#include <string>

using namespace std;

namespace tuber{
	
struct JoinStatement{
	string table;
	string statement;
	string joinMethod;
};
	
}

#endif