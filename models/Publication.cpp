#include "Publication.h"
#include <system/database/query/QueryBuilder.h>

Publication::Publication(){
    setModelAttributes("publications","publication_id",false);
}

void Publication::getPublicationList(RowCallback callback,Bundle* data){

    Publication::select({
                 "publications.publication_id",
                 "publications.publication_name"
                 })
            ->from("publications")
            ->get(callback,data);
}
