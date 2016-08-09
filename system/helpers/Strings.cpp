#include <system/helpers/Strings.h>
#include <sstream>

namespace tuber{

string Strings::toString(int val){
	stringstream ss;
	ss<<val;
	return ss.str();
}

string Strings::toString(double val){
	stringstream ss;
	ss<<val;
	return ss.str();
}

string Strings::toString(bool val){
	if(val){
		return string("true");
	} else {
		return string("false");
	}
}

string Strings::toString(const char* val){
	return string(val);
}

string Strings::join(vector<string> components,string glue){
	string joined;
	for(auto element: components){
		if(element != components.back() && element != ""){
			element += glue;
		}
		joined += element;
	}
	return joined;
}

string Strings::join(list<string> components,string glue){
	string joined;
	for(auto element: components){
		if(element != components.back() && element != ""){
			element += glue;
		}
		joined += element;
	}
	return joined;
}

vector<string> Strings::split(string str, char delimiter) {
	vector<string> internal;
	stringstream ss(str); // Turn the string into a stream.
	string tok;

	while(getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}

	return internal;
}

}
