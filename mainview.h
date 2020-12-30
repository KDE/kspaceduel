/*
    SPDX-FileCopyrightText: 1998-2001 Andreas Zehender <az@azweb.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef __MY_MAIN_VIEW_H
#define __MY_MAIN_VIEW_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QList>
#include <QRandomGenerator>
#include <QTimerEvent>
#include <QWidget>


class KToggleAction;
class KActionCollection;
class QGraphicsSimpleTextItem;

#include "dialogs.h"
#include "sprites.h"
class Ai;

#ifdef sun
#undef sun
#endif

class MyMainView:public QWidget
{
   Q_OBJECT
public:
   explicit MyMainView(QWidget *parent = nullptr);
   ~MyMainView() override;

   static KToggleAction *pauseAction;
   void setActionCollection(KActionCollection *a);

public Q_SLOTS:
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
Q_SIGNALS:
   void hitPoints(int pn,int hp);
   void energy(int pn,int en);
   void wins(int pn,int w);
   void setStatusText(const QString & str,int id);

protected:
   void resizeEvent(QResizeEvent *event) override;
   void timerEvent(QTimerEvent *event) override;
   void keyPressEvent(QKeyEvent *event) override;
   void keyReleaseEvent(QKeyEvent *event) override;
   void focusOutEvent (QFocusEvent * /*event*/) override;
   bool readSprites();
   SConfig modifyConfig(const SConfig &conf);
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

// SVG sprites
   QSvgRenderer* svgrender;
   // This could probably be gotten rid of, but it'll be kind of a pain
   QString powerupelements[PowerupSprite::PowerupNum];
   
   QMap<int, QList<QString> > animation;

   ShipSprite *ship[2];
   SunSprite *sun;
   QGraphicsSimpleTextItem *textSprite;

   QList<BulletSprite*> *bullets[2];
   QList<MineSprite*> *mines[2];
   QList<ExplosionSprite*> explosions;
   QList<PowerupSprite*> powerups;

   QRandomGenerator random;

//Ai
   Ai *ai[2];

};

#endif
