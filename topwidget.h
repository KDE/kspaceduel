#ifndef __MY_TOP_WIDGET_H
#define __MY_TOP_WIDGET_H

#include <ktmainwindow.h>
class KConfig;
#include "duelwidget.h"

class MyTopLevelWidget:public KTMainWindow
{
   Q_OBJECT
public:
   MyTopLevelWidget(const char *name=0);
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
};

#endif
