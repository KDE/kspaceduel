#ifndef __MY_TOP_WIDGET_H
#define __MY_TOP_WIDGET_H

#include <kmainwindow.h>

class PlayerInfo;
class MyMainView;

class MyTopLevelWidget:public KMainWindow
{
   Q_OBJECT
public:
   MyTopLevelWidget();
   void start();

private slots:
   void setStatusText(const QString & text,int id);
   void keySetup();
   void energy(int pn,int en);
   void hitPoints(int pn,int hp);
   void wins(int pn,int w);

protected:
   void initActions( );
   void initStatusBar( );
   void initGameWidgets();

private:
   PlayerInfo *playerinfo[2];
   MyMainView *playfield;
};

#endif
