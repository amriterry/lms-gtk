#ifndef _GTK_HELPER_H_
#define _GTK_HELPER_H_

#include <string>

#include <gtk/gtk.h>

using namespace std;

namespace tuber{

class Gtk{
public:
	static string getSelectedTreePath(GtkTreeView* view);
};

}

#endif