#include <algorithm>

#include <system/database/Binding.h>

#include <system/foundation/KeyNotFoundException.h>
#include <system/foundation/RunTimeException.h>

namespace tuber{

Binding::Binding(){
	this->m_pos = 1;
}

int Binding::getCurrentPosition(){
	return this->m_pos;
}

Binding* Binding::bindNull(string bindingFor,int pos){
	if(pos == -1){
		pos = m_pos;
		this->m_nulls.push_back(this->m_pos++);
	} else {
		this->m_nulls.push_back(pos);
	}
	this->setBindingMeta(pos,bindingFor,BindingType::GENERIC);
	return this;
}

Binding* Binding::bindInteger(int value,string bindingFor,int pos){
	if(pos == -1){
		pos = m_pos;
		this->m_integers[m_pos++] = value;
	} else {
		this->m_integers[pos];
	}
	this->setBindingMeta(pos,bindingFor,BindingType::INTEGER);
	return this;
}

Binding* Binding::bindBoolean(bool value,string bindingFor,int pos){
	if(pos == -1){
		pos = m_pos;
		this->m_booleans[m_pos++] = value;
	} else {
		this->m_booleans[pos] = value;
	}
	this->setBindingMeta(pos,bindingFor,BindingType::BOOLEAN);
	return this;
}

Binding* Binding::bindDouble(double value,string bindingFor,int pos){
	if(pos == -1){
		pos = m_pos;
		this->m_doubles[m_pos++] = value;
	} else {
		this->m_doubles[pos] = value;
	}
	this->setBindingMeta(pos,bindingFor,BindingType::DOUBLE);
	return this;
}

Binding* Binding::bindString(string value,string bindingFor,int pos){
	if(pos == -1){
		pos = m_pos;
		this->m_strings[m_pos++] = value;
	} else {
		this->m_strings[pos] = value;
	}
	this->setBindingMeta(pos,bindingFor,BindingType::STRING);
	return this;
}

bool Binding::hasNull(){
	return (this->m_nulls.size() != 0);
}

bool Binding::hasIntegers(){
	return (this->m_integers.size() != 0);
}

bool Binding::hasBooleans(){
	return (this->m_booleans.size() != 0);
}

bool Binding::hasStrings(){
	return (this->m_strings.size() != 0);
}

bool Binding::hasDoubles(){
	return (this->m_doubles.size() != 0);
}

bool Binding::hasNullAt(int pos){
	return (find(m_nulls.begin(),m_nulls.end(),pos) != m_nulls.end());
}

bool Binding::hasIntegerAt(int pos){
	return (this->m_integers.find(pos) != m_integers.end());
}

bool Binding::hasDoubleAt(int pos){
	return (this->m_doubles.find(pos) != m_doubles.end());
}

bool Binding::hasBooleanAt(int pos){
	return (this->m_booleans.find(pos) != m_booleans.end());
}

bool Binding::hasStringAt(int pos){
	return (this->m_strings.find(pos) != m_strings.end());
}

int Binding::getIntegerAt(int pos){
	if(hasIntegerAt(pos)){
		return m_integers[pos];
	} else {
		string message = "No Binding Found at Pos";
		throw RunTimeException(message);
	}
}

double Binding::getDoubleAt(int pos){
	if(hasDoubleAt(pos)){
		return m_doubles[pos];
	} else {
		string message = "No Binding Found at Pos";
		throw RunTimeException(message);
	}
}

bool Binding::getBooleanAt(int pos){
	if(hasBooleanAt(pos)){
		return m_booleans[pos];
	} else {
		string message = "No Binding Found at Pos";
		throw RunTimeException(message);
	}
}

string Binding::getStringAt(int pos){
	if(hasStringAt(pos)){
		return m_strings[pos];
	} else {
		string message = "No Binding Found at Pos";
		throw RunTimeException(message);
	}
}


vector<int> Binding::getNull(){
	return this->m_nulls;
}

map<int,int> Binding::getIntegers(){
	return this->m_integers;
}

map<int,bool> Binding::getBooleans(){
	return this->m_booleans;
}

map<int,string> Binding::getStrings(){
	return this->m_strings;
}

map<int,double> Binding::getDoubles(){
	return this->m_doubles;
}

bool Binding::hasBindingMetaFor(string bindingFor){
	return (this->m_bindingFor.find(bindingFor) != this->m_bindingFor.end());
}

void Binding::setBindingMeta(int pos,string bindingFor,BindingType dataType){
	BindingInfo info;
	info.bindingPos = pos;
	info.dataType = dataType;
	this->m_bindingFor[bindingFor].push_back(info);
}

vector<int> Binding::getPositionsFor(string bindingFor){
	vector<int> positions;
	if(m_bindingFor.find(bindingFor) != m_bindingFor.end()){
		vector<BindingInfo>& info = m_bindingFor[bindingFor];
		for(auto it:info){
			positions.push_back(it.bindingPos);
		}
	}
	return positions;
}

map<string,vector<BindingInfo>> Binding::getBindingMeta(){
	return this->m_bindingFor;
}

vector<BindingInfo> Binding::getBindingMetaFor(string bindingFor){
	if(hasBindingMetaFor(bindingFor)){
		return this->m_bindingFor[bindingFor];
	} else {
		throw KeyNotFoundException(bindingFor);
	}
}

vector<BindingInfo> Binding::getParsedBindingMeta(){
	vector<BindingInfo> info;
	if(hasBindingMetaFor("values")){
		info = m_bindingFor["values"];
	}

	if(hasBindingMetaFor("set")){
		info.insert(info.end(),m_bindingFor["set"].begin(),m_bindingFor["set"].end());
	}

	if(hasBindingMetaFor("where")){
		info.insert(info.end(),m_bindingFor["where"].begin(),m_bindingFor["where"].end());
	}

	if(hasBindingMetaFor("limit")){
		info.insert(info.end(),m_bindingFor["limit"].begin(),m_bindingFor["limit"].end());
	}

	return info;
}


}
