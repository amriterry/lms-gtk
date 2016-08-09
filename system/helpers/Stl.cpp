#include "Stl.h"

#include <algorithm>

namespace tuber{

bool Stl::hasValue(string needle,vector<string> haystack){
	return (find(haystack.begin(),haystack.end(),needle) != haystack.end());
}

}
