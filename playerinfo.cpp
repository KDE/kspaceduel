#include "playerinfo.h"
#include <qpixmap.h>
#include <QToolTip>
//Added by qt3to4:
#include <Q3Frame>
#include <klocale.h>
#include <kstandarddirs.h>


PlayerInfo::PlayerInfo(int pnr,QWidget *parent,const char *name)
      :Q3Frame(parent,name),
       lplayer(this),lenergy(this),lwins(this),
       hitpoints(2,this),energy(2,this),wins(2,this)
{
   setFixedWidth(45);
   setFrameStyle(Panel|Raised);
   QString str;
   int i;
   
   lplayer.setFrameStyle(Panel|Sunken);
   lplayer.setMargin(0);
   lplayer.setToolTip(i18n("Hit points"));
   lenergy.setFrameStyle(Panel|Sunken);
   lenergy.setMargin(0);
   lenergy.setToolTip(i18n("Energy"));
   lwins.setFrameStyle(Panel|Sunken);
   lwins.setMargin(0);
   lwins.setToolTip(i18n("Wins"));

   lplayer.setGeometry(5,5,35,35);
   lplayer.setIndent(0);
   lenergy.setGeometry(5,80,35,35);
   lenergy.setIndent(0);
   lwins.setGeometry(5,155,35,35);
   lwins.setIndent(0);

   for(i=0;i<4;i++)
   {
      str = QString::fromLatin1("sprites/playerinfo/ship%1%2.pnm")
        .arg(pnr+1)
        .arg(i);
      pix[i]=new QPixmap(KStandardDirs::locate("appdata", str));
   }

   lplayer.setPixmap(*pix[0]);
   currentPixmap=0;
   lenergy.setPixmap(QPixmap(KStandardDirs::locate("appdata", "sprites/playerinfo/energy.pnm")));
   lwins.setPixmap(QPixmap(KStandardDirs::locate("appdata", "sprites/playerinfo/win.pnm")));

   hitpoints.setGeometry(9,45,26,26);
   energy.setGeometry(9,120,26,26);
   wins.setGeometry(9,195,26,26);
   hitpoints.setFrameStyle(NoFrame);
   hitpoints.setToolTip(i18n("Hit points"));
   energy.setFrameStyle(NoFrame);
   energy.setToolTip(i18n("Energy"));
   wins.setFrameStyle(NoFrame);
   wins.setToolTip(i18n("Wins"));
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

#include "playerinfo.moc"
