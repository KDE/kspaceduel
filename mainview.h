#ifndef __MY_MAIN_VIEW_H
#define __MY_MAIN_VIEW_H

#include <qwidget.h>
#include <qimage.h>

class KToggleAction;
class KActionCollection;
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
   QCanvasPixmapArray* loadOldPixmapSequence(const QString& datapattern,
                           const QString& maskpattern, int framecount=1);
   void moveShips();
   void moveBullets();
   void moveMines();
   void moveExplosions();
   void calculatePowerups();
   void collisions();
private:
   KActionCollection *actionCollection;

   QCanvas field;
   QCanvasView view;

   SConfig customConfig,config;

   int timerID;
   bool playerKeyPressed[2][PlayerKeyNum];
   bool bulletShot[2];
   bool minePut[2];
   bool waitForStart;
   double gameEnd;
   double timeToNextPowerup;

// sprites
   QPtrList<QImage> shipImages;
   QPtrList<QPoint> points;
   QImage bulletImage;
   QCanvasPixmapArray *bulletsequence[2];
   QCanvasPixmapArray *shipsequence[2];
   QCanvasPixmapArray *explosionsequence;
   QCanvasPixmapArray *minesequence[2];
   QCanvasPixmapArray *mineexplosionsequence;
   QCanvasPixmapArray *powerupsequence[PowerupSprite::PowerupNum];


   ShipSprite *ship[2];
   SunSprite *sun;
   QCanvasText *textSprite;
   QPtrList<BulletSprite> *bullets[2];
   QPtrList<MineSprite> *mines[2];
   QPtrList<ExplosionSprite> explosions;
   QPtrList<PowerupSprite> powerups;

   KRandomSequence random;

//Ai
   Ai *ai[2];

};

#endif
