#ifndef __DUEL_WIDGET_H
#define __DUEL_WIDGET_H

#include <qwidget.h>
class KConfig;

class PlayerInfo;
class MyMainView;

class DuelWidget:public QWidget
{
   Q_OBJECT
public:
   DuelWidget(QWidget *parent=0,const char *name=0);
   void readConfig(KConfig *cfg);
public slots:
   void newGame();
   void newRound();
   void keySetup();
   void pause();
   void togglePause();
   void aiSetup();
   void hitpointSetup();
   void graphicSetup();
   void gameSetup();
   void writeConfig();
   void start();

   void energy(int pn,int en);
   void hitPoints(int pn,int hp);
   void wins(int pn,int w);
signals:   
   void setStatusText(const QString & str,int id);
private:
   MyMainView *playfield;
   PlayerInfo *playerinfo[2];
};

#endif
