#ifndef __MY_TOP_WIDGET_H
#define __MY_TOP_WIDGET_H

#include <kmainwindow.h>

class KConfig;
class DuelWidget;
class KToggleAction;


class MyTopLevelWidget:public KMainWindow
{
   Q_OBJECT
public:
   MyTopLevelWidget();
   ~MyTopLevelWidget( );
   void start();
public slots:
   void setStatusText(const QString & text,int id);
   void quit();
   void saveOptions();
   void keySetup();
   void showToolBar( );
   void showStatusBar( );
protected:
   void readConfig(KConfig *cfg);
   void writeConfig(KConfig *cfg);
   void initActions( );
   void initStatusBar( );
private:
   DuelWidget* wview;
   KToggleAction *toolbarAct;
   KToggleAction *statusbarAct;
};

#endif
