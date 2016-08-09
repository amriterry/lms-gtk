#ifndef _SAVE_PUBLICATION_CONTROLLER_H_
#define _SAVE_PUBLICATION_CONTROLLER_H_

#include <system/controller/Controller.h>

using namespace std;
using namespace tuber;

class Publication;

class SavePublicationController: public Controller{
public:
	void onCreate(Bundle* package);
	void onStart();
	void onStop();
private:
	Publication* m_publication;
};

#endif