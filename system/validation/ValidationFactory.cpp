#include <system/validation/ValidationFactory.h>

namespace tuber{

ValidationFactory::ValidationFactory(){
	this->m_data = BundleFacade::get();
}

Bundle* ValidationFactory::fetchValidated(){
	return this->m_data;
}

ValidationFactory* ValidationFactory::addForValidation(Bundle* package){
	this->m_data = package;
	return this;
}

ValidationFactory* ValidationFactory::addRule(string key,string value,string rule){
	this->m_data->putExtra(key,value);
	this->addRule(key,rule);
	return this;
}

ValidationFactory* ValidationFactory::addRule(string key,int value,string rule){
	this->m_data->putExtra(key,value);
	this->addRule(key,rule);
	return this;
}

ValidationFactory* ValidationFactory::addRule(string key,double value,string rule){
	this->m_data->putExtra(key,value);
	this->addRule(key,rule);
	return this;
}

ValidationFactory* ValidationFactory::addRule(string key,string rule){
	this->m_keyRule.insert({key,rule});
	return this;
}

bool ValidationFactory::run(){
	for(auto rule:m_keyRule){
		rule.first;
		rule.second;
	}
	return true;
}

}