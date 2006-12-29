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

#ifndef __MY_MAIN_VIEW_H
#define __MY_MAIN_VIEW_H

#include <QWidget>
//Added by qt3to4:
#include <QTimerEvent>
#include <QResizeEvent>
#include <QKeyEvent>
#include <Q3PtrList>
#include <QLinkedList>

#include <QGraphicsScene>
#include <QGraphicsView>

class KToggleAction;
class KActionCollection;
class QGraphicsSimpleTextItem;
#include <krandomsequence.h>

#include "sprites.h"
#include "dialogs.h"
class Ai;

#ifdef sun
#undef sun
#endif

class MyMainView:public QWidget
{
   Q_OBJECT
public:
   MyMainView(QWidget *parent=0);
   ~MyMainView();

   static KToggleAction *pauseAction;
   void setActionCollection(KActionCollection *a);
   bool readSprites();

public slots:
   void newRound();
   void newGame();
   void togglePause( );
   void pause();
   void resume();
   void start();
   void stop();
   void gameSetup();
   void closeSettings();
   void readConfig();
   void writeConfig();
signals:
   void hitPoints(int pn,int hp);
   void energy(int pn,int en);
   void wins(int pn,int w);
   void setStatusText(const QString & str,int id);

protected:
   virtual void resizeEvent(QResizeEvent *event);
   virtual void timerEvent(QTimerEvent *event);
   virtual void keyPressEvent(QKeyEvent *event);
   virtual void keyReleaseEvent(QKeyEvent *event);
   SConfig modifyConfig(SConfig conf);
   void moveShips();
   void moveBullets();
   void moveMines();
   void moveExplosions();
   void calculatePowerups();
   void collisions();
private:
   KActionCollection *actionCollection;

   QGraphicsScene field;
   QGraphicsView view;

   SConfig customConfig,config;

   int timerID;
   bool playerKeyPressed[2][PlayerKeyNum];
   bool bulletShot[2];
   bool minePut[2];
   bool waitForStart;
   double gameEnd;
   double timeToNextPowerup;

// sprites
   Q3PtrList<QImage> shipImages;
   Q3PtrList<QPoint> points;
   
   QPixmap *sunpixmap;
   QPixmap *shippixmap[2];
   QPixmap *bulletpixmap[2];
   QPixmap *poweruppixmap[PowerupSprite::PowerupNum];
   
   QMap<int, QList<QPixmap> > animation;

   ShipSprite *ship[2];
   SunSprite *sun;
   QGraphicsSimpleTextItem *textSprite;
   Q3PtrList<BulletSprite> *bullets[2];
   Q3PtrList<MineSprite> *mines[2];
   Q3PtrList<ExplosionSprite> explosions;
   Q3PtrList<PowerupSprite> powerups;

   KRandomSequence random;

//Ai
   Ai *ai[2];

};

#endif
