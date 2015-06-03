#ifndef _I_BINDABLE_H_
#define _I_BINDABLE_H_

#include <string>

using namespace std;

namespace tuber{

class IBindable{
public:
    virtual ~IBindable(){}

    void setIdentifier(string bindingKey,int resolveId){
    	this->m_bindingKey = bindingKey;
    	this->m_resolveId = resolveId;
    }

    int getResolveId(){
    	return this->m_resolveId;
    }

    string getBindingKey(){
    	return this->m_bindingKey;
    }
protected:
	IBindable(){}
private:
	int m_resolveId;
	string m_bindingKey;
};

}

#endif //_I_BINDABLE_H_
