#ifndef _BINDING_H_
#define _BINDING_H_

#include <map>
#include <vector>
#include <string>

#include <system/support/IBindable.h>

namespace tuber{

class Binding: public IBindable{
public:
	Binding();
	int getCurrentPosition();
	Binding* bindNull(int pos = -1);
	Binding* bindInteger(int value,int pos = -1);
	Binding* bindBoolean(bool value,int pos = -1);
	Binding* bindDouble(double value,int pos = -1);
	Binding* bindString(string value,int pos = -1);
	
	bool hasNull();
	bool hasIntegers();
	bool hasBooleans();
	bool hasStrings();
	bool hasDoubles();

	vector<int> getNull();
	map<int,int> getIntegers();
	map<int,bool> getBooleans();
	map<int,string> getStrings();
	map<int,double> getDoubles();
protected:
	vector<int> m_nulls;
	map<int,int> m_integers;
	map<int,bool> m_booleans;
	map<int,string> m_strings;
	map<int,double> m_doubles;

	int m_pos;
};

}

#endif