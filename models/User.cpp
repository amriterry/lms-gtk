#include <gtk/gtk.h>

#include <models/User.h>

using namespace tuber;

User::User(){
    g_message("User: User model created");
	this->setModelAttributes("users","user_id",false);
}
