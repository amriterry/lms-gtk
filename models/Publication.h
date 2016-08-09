#ifndef _PUBLICATION_H_
#define _PUBLICATION_H_

#include <system/database/model/Model.h>

using namespace std;
using namespace tuber;

class Publication: public Model{
public:
    Publication();
    static void getPublicationList(RowCallback callback,Bundle* data = nullptr);
};

#endif