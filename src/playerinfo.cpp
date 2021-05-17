/*
    SPDX-FileCopyrightText: 1998-2001 Andreas Zehender <az@azweb.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "playerinfo.h"

#include <QIcon>
#include <KLocalizedString>
#include <QStandardPaths>


QPixmap* PlayerInfo::pplayer[6];

void PlayerInfo::loadPixmaps()
{
    pplayer[0] = new QPixmap(QIcon::fromTheme( QStringLiteral( "battery-discharging-100" )).pixmap(32,32));
    pplayer[1] = new QPixmap(QIcon::fromTheme( QStringLiteral( "battery-discharging-080" )).pixmap(32,32));
    pplayer[2] = new QPixmap(QIcon::fromTheme( QStringLiteral( "battery-discharging-060" )).pixmap(32,32));
    pplayer[3] = new QPixmap(QIcon::fromTheme( QStringLiteral( "battery-discharging-040" )).pixmap(32,32));
    pplayer[4] = new QPixmap(QIcon::fromTheme( QStringLiteral( "battery-discharging-020" )).pixmap(32,32));
    pplayer[5] = new QPixmap(QIcon::fromTheme( QStringLiteral( "battery-discharging-000" )).pixmap(32,32));
}

PlayerInfo::PlayerInfo(int pnr,QWidget *parent)
      :QFrame(parent),
       lplayer(this),lenergy(this),lwins(this),
       hitpoints(2,this),energy(2,this),wins(2,this)
{
   setFixedWidth(45);
   QString str;
//   int i;

   QPalette palette;

   lplayer.setFrameStyle(NoFrame);
   lplayer.setContentsMargins(0, 0, 0, 0);
   lplayer.setToolTip(i18n("Hit points"));
   lenergy.setFrameStyle(NoFrame);
   lenergy.setContentsMargins(0, 0, 0, 0);
   lenergy.setToolTip(i18n("Energy"));
   lwins.setFrameStyle(NoFrame);
   lwins.setContentsMargins(0, 0, 0, 0);
   lwins.setToolTip(i18n("Wins"));

   lplayer.setGeometry(5,5,35,35);
   lplayer.setIndent(0);
   lenergy.setGeometry(5,80,32,32);
   lenergy.setIndent(0);
   lwins.setGeometry(5,152,35,35);
   lwins.setIndent(0);

/*   for(i=0;i<4;i++)
   {
      str = QString::fromLatin1("sprites/playerinfo/ship%1%2.png")
        .arg(pnr+1)
        .arg(i);
      pix[i]=new QPixmap(QStandardPaths::locate(QStandardPaths::AppDataLocation, str));
   }*/

   lplayer.setPixmap(*(pplayer[0]));
   currentPixmap=0;
   //lenergy.setPixmap(QPixmap(QStandardPaths::locate(QStandardPaths::AppDataLocation, "sprites/playerinfo/energy.png")));
   lenergy.setPixmap(QIcon::fromTheme( QStringLiteral( "battery" )).pixmap(32,32));;
   lwins.setPixmap(QPixmap(QStandardPaths::locate(QStandardPaths::AppDataLocation, QStringLiteral( "sprites/playerinfo/win.png" ))));

   hitpoints.setGeometry(9,45,26,26);
   energy.setGeometry(9,120,26,26);
   wins.setGeometry(9,195,26,26);
   hitpoints.setFrameStyle(NoFrame);
   hitpoints.setToolTip(i18n("Hit points"));
   hitpoints.setSegmentStyle(QLCDNumber::Flat);
   energy.setFrameStyle(NoFrame);
   energy.setToolTip(i18n("Energy"));
   energy.setSegmentStyle(QLCDNumber::Flat);
   wins.setFrameStyle(NoFrame);
   wins.setToolTip(i18n("Wins"));
   wins.setSegmentStyle(QLCDNumber::Flat);
   if (pnr==0)
   {
      palette.setColor(hitpoints.foregroundRole(), QColor(251,37,37));
   } else
   {
      palette.setColor(hitpoints.foregroundRole(), QColor(61,23,233));
   }
   hitpoints.setPalette(palette);
   energy.setPalette(palette);
   wins.setPalette(palette);
}

void PlayerInfo::setHitpoints(int h)
{
   int p = 5-((h+19)/20);
   hitpoints.display(h);
   if(p!=currentPixmap)
   {
      lplayer.setPixmap(*(pplayer[p]));
      currentPixmap=p;
   }
}

void PlayerInfo::setEnergy(int e)
{
   energy.display(e);
}

void PlayerInfo::setWins(int w)
{
   // if the LCD range overflows, adjust its range
   if (wins.checkOverflow(w))
   {
      wins.setDigitCount(wins.digitCount()+1);
   }
   // if the LCD is set to 0 (start new game), also reset the range
   if (w==0)
   {
      wins.setDigitCount(2);
   }
   wins.display(w);
}


