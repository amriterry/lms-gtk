#include "TestController.h"

#include <iostream>

#include <system/database/model/Model.h>
#include <system/facades/Facades.h>
#include <system/database/connections/Connection.h>
#include <system/database/Binding.h>
#include <system/bundle/Bundle.h>

#include <models/User.h>
#include <system/database/query/QueryBuilder.h>

using namespace std;
using namespace tuber;

void TestController::onCreate(Bundle* package){
	g_message("TestController: onCreate");
}

void TestController::onStart(){
    g_message("TestController: onStart");
    //Binding* binding = App::get()->resolve<Binding>("db.query.binding");
   //binding->bindInteger(-1);
    //QueryResult result = DB::get()->connection()->selectQuery("SELECT * FROM users WHERE users.user_id = ? ORDER BY users.name DESC",binding);
    /*User* user = User::find<User>(12);
    if(!user->fetch()->empty()){
        g_message("Row Found");
        cout<<user->fetch()->getString("name")<<endl;
        user->getDirty()
            ->putExtra("name",string("Amrit jpt"))
            ->putExtra("username","terryh")
            ->putExtra("status_id",2);
        if(!user->saveRow()){
            g_message("Row not saved");
        }
    }*/

    Bundle* package = BundleFacade::get();

    package->commitSucide();

    /*User* user = User::find<User>(2);
    if(user->deleteRow()){
        g_message("Row deleted");
    } else {
        g_message("Row not deleted");
    }*/


    //user->saveRow();

    //DB::rawSelect("select * from users");
    /*QueryRow result = User::select({"users.user_id","users.name","user_reln.data"})
                                    ->from("users")
                                    ->join("user_reln","user_reln.user_id = users.user_id","left")
                                    ->where("users.user_id","=",2)
                                    ->first();

    //for(auto result: resultUser){
        if(!result->empty()){
            cout<<result->getInt("user_id")<<" = "<<result->getString("name")<<" = "<<result->getSafeString("data")<<endl;

        }
    //}*/
}
