#include "playerinfo.h"
#include <qpixmap.h>
#include <qtooltip.h>
#include <klocale.h>

#include "defines.h"

PlayerInfo::PlayerInfo(int pnr,QWidget *parent,const char *name)
      :QFrame(parent,name),
       lplayer(this),lenergy(this),lwins(this),
       hitpoints(2,this),energy(2,this),wins(2,this)
{
   setFixedWidth(45);
   setFrameStyle(Panel|Raised);
   QString str;
   int i;
   
   lplayer.setFrameStyle(Panel|Sunken);
   lplayer.setMargin(0);
   QToolTip::add(&lplayer,i18n("Hit points"));
   lenergy.setFrameStyle(Panel|Sunken);
   lenergy.setMargin(0);
   QToolTip::add(&lenergy,i18n("Energy"));
   lwins.setFrameStyle(Panel|Sunken);
   lwins.setMargin(0);
   QToolTip::add(&lwins,i18n("Wins"));

   lplayer.setGeometry(5,5,35,35);
   lenergy.setGeometry(5,80,35,35);
   lwins.setGeometry(5,155,35,35);

   for(i=0;i<4;i++)
   {
      str.sprintf("sprites/playerinfo/ship%i%i.pnm",pnr+1,i);
      pix[i]=new QPixmap(MV_PREFIX+str);
   }

   lplayer.setPixmap(*pix[0]);
   currentPixmap=0;
   lenergy.setPixmap(QPixmap(MV_PREFIX+"sprites/playerinfo/energy.pnm"));
   lwins.setPixmap(QPixmap(MV_PREFIX+"sprites/playerinfo/win.pnm"));

   hitpoints.setGeometry(9,45,26,26);
   energy.setGeometry(9,120,26,26);
   wins.setGeometry(9,195,26,26);
   hitpoints.setFrameStyle(NoFrame);
   QToolTip::add(&hitpoints,i18n("Hit points"));
   energy.setFrameStyle(NoFrame);
   QToolTip::add(&energy,i18n("Energy"));
   wins.setFrameStyle(NoFrame);
   QToolTip::add(&wins,i18n("Wins"));
}
       
void PlayerInfo::setHitpoints(int h)
{
   int p=3-h/25;
   hitpoints.display(h);
   if(p!=currentPixmap)
   {
      lplayer.setPixmap(*(pix[p]));
      currentPixmap=p;
   }
}

void PlayerInfo::setEnergy(int e)
{
   energy.display(e);
}

void PlayerInfo::setWins(int w)
{
   wins.display(w);
}

