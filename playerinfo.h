#ifndef __PLAYER_INFO_H
#define __PLAYER_INFO_H

#include <qframe.h>
#include <qlabel.h>
class QPixmap;
#include <qlcdnumber.h>
#include <qcolor.h>

class PlayerInfo:public QFrame
{
   Q_OBJECT
public:
   PlayerInfo(int pnr,QWidget *parent=0,const char *name=0);
public slots:
   void setHitpoints(int h);
   void setEnergy(int e);
   void setWins(int w);
private:
   QPixmap* pix[4];
   int currentPixmap;
   QLabel lplayer,lenergy,lwins;
   QLCDNumber hitpoints,energy,wins;
};

#endif
