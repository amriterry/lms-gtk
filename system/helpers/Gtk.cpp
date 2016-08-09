#include "Gtk.h"

namespace tuber{

string Gtk::getSelectedTreePath(GtkTreeView* treeview){
	string stringPath;
	GtkTreeIter iter;
	char* pathCString;
	GtkTreeSelection* selected = gtk_tree_view_get_selection(treeview);
	GtkTreeModel* model = gtk_tree_view_get_model(treeview);

	if(gtk_tree_selection_get_selected(selected,&model,&iter)){
		GtkTreePath* path = gtk_tree_model_get_path(model,&iter);
		pathCString = gtk_tree_path_to_string(path);
		g_free(path);

		stringPath = string(pathCString);
	}
	return stringPath;
}

}
