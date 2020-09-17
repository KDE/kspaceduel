/*
    Copyright (C) 1998-2001 Andreas Zehender <az@azweb.de>

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
   ~MyMainView();

   static KToggleAction *pauseAction;
   void setActionCollection(KActionCollection *a);

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
   void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
   void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
   void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
   void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
   void focusOutEvent (QFocusEvent * /*event*/) Q_DECL_OVERRIDE;
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
