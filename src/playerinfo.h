/*
    SPDX-FileCopyrightText: 1998-2001 Andreas Zehender <az@azweb.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef __PLAYER_INFO_H
#define __PLAYER_INFO_H

#include <QFrame>
#include <QLabel>
#include <QLCDNumber>
class QPixmap;

class PlayerInfo:public QFrame
{
   Q_OBJECT
public:
   explicit PlayerInfo(int pnr,QWidget *parent = nullptr);
   static void loadPixmaps();
public Q_SLOTS:
   void setHitpoints(int h);
   void setEnergy(int e);
   void setWins(int w);
private:
   static QPixmap* pplayer[6];
   int currentPixmap;
   QLabel lplayer,lenergy,lwins;
   QLCDNumber hitpoints,energy,wins;
};

#endif
