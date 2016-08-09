#ifndef _BINDING_H_
#define _BINDING_H_

#include <map>
#include <vector>
#include <string>

#include <system/support/IBindable.h>

#include <system/database/BindingInfo.h>

namespace tuber{

class Binding: public IBindable{
public:
	Binding();
	int getCurrentPosition();
	Binding* bindNull(string bindingFor = "where",int pos = -1);
	Binding* bindInteger(int value,string bindingFor = "where",int pos = -1);
	Binding* bindBoolean(bool value,string bindingFor = "where",int pos = -1);
	Binding* bindDouble(double value,string bindingFor = "where",int pos = -1);
	Binding* bindString(string value,string bindingFor = "where",int pos = -1);

	bool hasNull();
	bool hasIntegers();
	bool hasBooleans();
	bool hasStrings();
	bool hasDoubles();

	bool hasNullAt(int pos);
	bool hasIntegerAt(int pos);
	bool hasDoubleAt(int pos);
	bool hasBooleanAt(int pos);
	bool hasStringAt(int pos);

	vector<int> getNull();
	map<int,int> getIntegers();
	map<int,bool> getBooleans();
	map<int,string> getStrings();
	map<int,double> getDoubles();

	int getIntegerAt(int pos);
	double getDoubleAt(int pos);
	bool getBooleanAt(int pos);
	string getStringAt(int pos);

	bool hasBindingMetaFor(string bindingFor);
	map<string,vector<BindingInfo>> getBindingMeta();
	vector<BindingInfo> getBindingMetaFor(string bindingFor);
	vector<int> getPositionsFor(string bindingFor);

	vector<BindingInfo> getParsedBindingMeta();
protected:
	void setBindingMeta(int pos,string bindingFor,BindingType dataType);

	vector<int> m_nulls;
	map<int,int> m_integers;
	map<int,bool> m_booleans;
	map<int,string> m_strings;
	map<int,double> m_doubles;
	map<string,vector<BindingInfo>> m_bindingFor;
	int m_pos;
};

}

#endif
