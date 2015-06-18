#include <system/database/Binding.h>

namespace tuber{

Binding::Binding(){
	this->m_pos = 1;
}

int Binding::getCurrentPosition(){
	return this->m_pos;
}

Binding* Binding::bindNull(int pos){
	if(pos == -1){
		this->m_nulls.push_back(this->m_pos++);
	} else {
		this->m_nulls.push_back(pos);
	}
	return this;
}

Binding* Binding::bindInteger(int value,int pos){
	if(pos == -1){
		this->m_integers[m_pos++] = value;
	} else {
		this->m_integers[pos];
	}
	return this;
}

Binding* Binding::bindBoolean(bool value,int pos){
	if(pos == -1){
		this->m_booleans[m_pos++] = value;
	} else {
		this->m_booleans[pos] = value;
	}
	return this;
}

Binding* Binding::bindDouble(double value,int pos){
	if(pos == -1){
		this->m_doubles[m_pos++] = value;
	} else {
		this->m_doubles[pos] = value;
	}
	return this;
}

Binding* Binding::bindString(string value,int pos){
	if(pos == -1){
		this->m_strings[m_pos++] = value;
	} else {
		this->m_strings[pos] = value;
	}
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


}
