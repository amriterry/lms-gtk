#ifndef _VALIDATION_FACTORY_H_
#define _VALIDATION_FACTORY_H_

#include <system/validation/ValidatorRule.h>

namespace tuber{

class ValidationFactory{
public:
	ValidationFactory();
	Bundle* fetchValidated();
	ValidationFactory* addForValidation(Bundle* package);
	ValidationFactory* addRule(string key,string value,string rule);
	ValidationFactory* addRule(string key,int value,string rule);
	ValidationFactory* addRule(string key,double value,string rule);
	ValidationFactory* addRule(string key,string rule);
	bool run();
private:
	Bundle* m_data;
	map<string,string> m_keyRule;

	const static vector<string> m_rules = {
		"required",
		"integer",
		"double",
		"numeric",
		"string",
		"confirmed",
		"min",
		"max",
		"unique"
	};
};

}

#endif