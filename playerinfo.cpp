/* Copyright (C) 1998-2001 Andreas Zehender <az@azweb.de>

This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <QToolTip>

#include <klocale.h>
#include <kstandarddirs.h>

#include "playerinfo.h"


PlayerInfo::PlayerInfo(int pnr,QWidget *parent)
      :QFrame(parent),
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
      str = QString::fromLatin1("sprites/playerinfo/ship%1%2.png")
        .arg(pnr+1)
        .arg(i);
      pix[i]=new QPixmap(KStandardDirs::locate("appdata", str));
   }

   lplayer.setPixmap(*pix[0]);
   currentPixmap=0;
   lenergy.setPixmap(QPixmap(KStandardDirs::locate("appdata", "sprites/playerinfo/energy.png")));
   lwins.setPixmap(QPixmap(KStandardDirs::locate("appdata", "sprites/playerinfo/win.png")));

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
