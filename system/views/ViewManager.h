#ifndef _VIEW_MANAGER_H_
#define _VIEW_MANAGER_H_

#include <gtk/gtk.h>

#include <system/foundation/Service.h>

namespace tuber{

class Application;
class Controller;

class ViewManager: public Service{
public:
	ViewManager(Application* app);
	void setDefaultStage(int stageId);
	GtkWindow* getDefaultStage();
	GtkWidget* setScene(Controller* controller,string viewPath,string rootName = "root",bool topLevel = false);
	GtkWidget* setScene(GtkWidget* scene,bool topLevel = false);
	GtkWidget* setScene(GtkWindow* stage,GtkWidget* scene,bool topLevel = false);
private:
	Application* m_app;
	GtkApplication* m_gtkApp;
	int m_defaultStageId;
};

}

#endif
