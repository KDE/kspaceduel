#ifndef __MY_TOP_WIDGET_H
#define __MY_TOP_WIDGET_H

#include <ktopwidget.h>
#include <kmenubar.h>
#include <kconfig.h>
#include "duelwidget.h"

class MyTopLevelWidget:public KTopLevelWidget
{
   Q_OBJECT
public:
   MyTopLevelWidget(const char *name=0);
   void start();
public slots:
   void setStatusText(const char* text,int id);
   void quit();
protected:
   void readConfig(KConfig *cfg);
   void writeConfig(KConfig *cfg);
private:
   KMenuBar menu;
   KStatusBar statusbar;
   DuelWidget wview;
};

#endif
