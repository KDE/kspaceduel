#ifndef __PLAYER_INFO_H
#define __PLAYER_INFO_H

#include <QFrame>
#include <QLabel>
//Added by qt3to4:
#include <QPixmap>
class QPixmap;
#include <qlcdnumber.h>
#include <QColor>

class PlayerInfo:public QFrame
{
   Q_OBJECT
public:
   PlayerInfo(int pnr,QWidget *parent=0);
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
