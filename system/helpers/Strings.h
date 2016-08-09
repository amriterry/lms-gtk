#ifndef _STRINGS_H_
#define _STRINGS_H_

#include <string>
#include <list>
#include <vector>

using namespace std;

namespace tuber{

class Strings{
public:
	static string toString(int val);
	static string toString(double val);
	static string toString(bool val);
	static string toString(const char* val);

	static string join(vector<string> components,string glue);
	static string join(list<string> components,string glue);
	static vector<string> split(string str,char delimiter);
};

};

#endif