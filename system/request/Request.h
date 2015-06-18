#ifndef _REQUEST_H_
#define _REQUEST_H_

#include <string>

#include <gtk/gtk.h>

#include <system/foundation/Service.h>

using namespace std;

namespace tuber{

class Bundle;

/**
 * @todo Make request object generic to create, pause etc actions of various object such as controller
 */
class Request: public Service{
public:
    Request();
	void setActionKey(string actionKey);
	string getActionKey();
	void carryPackage(Bundle* package);
	Bundle* getPackage();
	~Request();
private:
	string m_actionKey;
	Bundle* m_package;
};

}

#endif
