#ifndef __MY_TOP_WIDGET_H
#define __MY_TOP_WIDGET_H

#include <ktmainwindow.h>
#include <kmenubar.h>
#include <kaccel.h>
#include <kaccelmenu.h>
#include <kconfig.h>
#include "duelwidget.h"

class MyTopLevelWidget:public KTMainWindow
{
   Q_OBJECT
public:
   MyTopLevelWidget(const char *name=0);
   void start();
public slots:
   void setStatusText(const char* text,int id);
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
   DuelWidget wview;
};

#endif
