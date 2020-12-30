/*
    SPDX-FileCopyrightText: 1998-2001 Andreas Zehender <az@azweb.de>
    SPDX-FileCopyrightText: 2006-2007 Dirk Rathlev <dirkrathlev@gmx.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "mainview.h"
#include "ai.h"
#include "options.h"

#include <KActionCollection>
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KToggleAction>

#include <QAbstractEventDispatcher>
#include <QBrush>
#include <QFontDatabase>
#include <QGraphicsSimpleTextItem>
#include <QKeyEvent>
#include <QKeySequence>
#include <QResizeEvent>
#include <QSvgRenderer>
#include <QStandardPaths>

#include <math.h>

KToggleAction *MyMainView::pauseAction = nullptr;

static struct
 {
     int id;
     const char *path;
 }
 kspd_animations [] =
 {
     { ID_EXPLOSION, "explos%1"},
     { ID_MINE1, "mine_red%1"},
     { ID_MINE2, "mine_blue%1"},
     { ID_MINEEXPLO, "mineex%1"},
     { 0, nullptr}
 };

MyMainView::MyMainView(QWidget *parent)
    :QWidget(parent),
    field(this),//0,0,DEF_WIDTH,DEF_HEIGHT),
    view(&field,this)
{
   int i,p;
   setMinimumSize(600,400);
   random.seed(QRandomGenerator::global()->generate());
   QPixmap backgr(QStandardPaths::locate(QStandardPaths::AppDataLocation, QStringLiteral(MV_BACKGROUND)));

   field.setBackgroundBrush(QBrush(backgr));
   view.setCacheMode(QGraphicsView::CacheBackground);

   setFocusPolicy(Qt::StrongFocus);
   // FIXME: is this needed anymore?
   //view.setResizePolicy(Q3ScrollView::AutoOne);
   view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   view.setFrameStyle(QFrame::NoFrame);

   for(p=0;p<2;++p)
   {
      for(i=0;i<PlayerKeyNum;++i)
         playerKeyPressed[p][i]=false;
      bulletShot[p]=false;
      minePut[p]=false;
   }

   svgrender = new QSvgRenderer(QStandardPaths::locate(QStandardPaths::AppDataLocation, QStringLiteral(MV_SVG_FILE)));

   sun=new SunSprite(svgrender,QStringLiteral( MV_SUN ));
   field.addItem(sun);
   sun->setPos(QPointF(width()/2-1-(sun->width()/2),
                       height()/2-1-(sun->height()/2)));

   powerupelements[0] = QStringLiteral( MV_POWERMINE );
   powerupelements[1] = QStringLiteral( MV_POWERBULLET );
   powerupelements[2] = QStringLiteral( MV_POWERSHIELD );
   powerupelements[3] = QStringLiteral( MV_POWERENERGY );

   ship[0]=new ShipSprite(svgrender, QStringLiteral( MV_SHIP1 ), 0);
   ship[1]=new ShipSprite(svgrender, QStringLiteral( MV_SHIP2 ), 1);
   field.addItem(ship[0]);
   field.addItem(ship[1]);

   readSprites();

   for(i=0;i<2;++i)
   {
      // ship[i]->setBoundsAction(QwRealMobileSprite::Wrap);
      ship[i]->hide();
      bullets[i]=new QList<BulletSprite*>;
      mines[i]=new QList<MineSprite*>;

   }

   waitForStart=false;
   textSprite = nullptr;
   readConfig();
}

MyMainView::~MyMainView()
{
   int i;
   QAbstractEventDispatcher::instance()->unregisterTimers(this);

   for(i=0;i<2;i++)
   {
      delete ai[i];
      qDeleteAll(*mines[i]);
      delete mines[i];
      qDeleteAll(*bullets[i]);
      delete bullets[i];
   }

   qDeleteAll(powerups);
   powerups.clear();

   qDeleteAll(explosions);
   explosions.clear();

   writeConfig();
   delete svgrender;
}

void MyMainView::setActionCollection(KActionCollection *a)
{
   actionCollection = a;
}

/* Assumes that there are no gaps between animation frames. ie 1,2,3 will only have frames 1&2
   recognized. It also assumes that there is at least one frame. */
// FIXME: Add Check for existence of first frame
// TODO: Add support for missing frames (ie 1,2,5)
bool MyMainView::readSprites()
{
   int i = 0;
   while ( kspd_animations[i].id )
   {
      QList<QString> anim;
      short frame = 0;
      QString element =QLatin1String( kspd_animations[i].path );
      QString elem = element.arg(frame, 2, 10, QLatin1Char('0'));
      do
      {
	 anim.append(elem);
	 frame++;
	 elem = element.arg(frame, 2, 10, QLatin1Char('0'));
      } while ( svgrender->elementExists(elem) );
      animation.insert( kspd_animations[i].id, anim );
      i++;
   }

   // FIXME: Perform test!
   return true;
}

void MyMainView::readConfig()
{
   KConfigGroup game(KSharedConfig::openConfig(), "Game");
   int i;

   customConfig.gamespeed=game.readEntry("gamespeed",
                                                  predefinedConfig[0].gamespeed);

   customConfig.gravity=
      game.readEntry("gravity",predefinedConfig[0].gravity);
   customConfig.acc=
      game.readEntry("acceleration",predefinedConfig[0].acc);
   customConfig.bulletDamage=
      game.readEntry("bulletDamage",predefinedConfig[0].bulletDamage);
   customConfig.bulletLifeTime=
      game.readEntry("bulletLifeTime",predefinedConfig[0].bulletLifeTime);
   customConfig.bulletReloadTime=
      game.readEntry("bulletReloadTime",predefinedConfig[0].bulletReloadTime);
   customConfig.mineDamage=
      game.readEntry("mineDamage",predefinedConfig[0].mineDamage);
   customConfig.shipDamage=
      game.readEntry("shipDamage",predefinedConfig[0].shipDamage);
   customConfig.maxBullets=
      game.readEntry("maxBullets",predefinedConfig[0].maxBullets);
   customConfig.maxMines=
      game.readEntry("maxMines",predefinedConfig[0].maxMines);
   customConfig.mineReloadTime=
      game.readEntry("mineReloadTime",predefinedConfig[0].mineReloadTime);
   customConfig.rotationSpeed=
      game.readEntry("rotationSpeed",predefinedConfig[0].rotationSpeed);
   customConfig.shotSpeed=
      game.readEntry("shotSpeed",predefinedConfig[0].shotSpeed);
   customConfig.energyNeed=
      game.readEntry("accEnergyNeed",predefinedConfig[0].energyNeed);
   customConfig.rotationEnergyNeed=
      game.readEntry("rotationEnergyNeed",predefinedConfig[0].rotationEnergyNeed);
   customConfig.sunEnergy=
      game.readEntry("sunEnergy",predefinedConfig[0].sunEnergy);
   customConfig.mineActivateTime=
      game.readEntry("mineActivateTime",predefinedConfig[0].mineActivateTime);
   customConfig.mineFuel=
      game.readEntry("mineFuel",predefinedConfig[0].mineFuel);
   customConfig.shotEnergyNeed=
      game.readEntry("shotEnergyNeed",predefinedConfig[0].shotEnergyNeed);
   customConfig.mineEnergyNeed=
      game.readEntry("mineEnergyNeed",predefinedConfig[0].mineEnergyNeed);
   customConfig.startPosX=
      game.readEntry("startPosX",predefinedConfig[0].startPosX);
   customConfig.startPosY=
      game.readEntry("startPosY",predefinedConfig[0].startPosY);
   customConfig.startVelX=
      game.readEntry("startVelX",predefinedConfig[0].startVelX);
   customConfig.startVelY=
      game.readEntry("startVelY",predefinedConfig[0].startVelY);
   customConfig.powerupLifeTime=
      game.readEntry("powerupLifeTime",predefinedConfig[0].powerupLifeTime);
   customConfig.powerupRefreshTime=
      game.readEntry("powerupRefreshTime",predefinedConfig[0].powerupRefreshTime);
   customConfig.powerupShieldAmount=
      game.readEntry("powerupShieldAmount",
                              predefinedConfig[0].powerupShieldAmount);
   customConfig.powerupEnergyAmount=
      game.readEntry("powerupEnergyAmount",
                              predefinedConfig[0].powerupEnergyAmount);

   if(Options::lastConfig() < predefinedConfigNum)
      config=modifyConfig(predefinedConfig[Options::lastConfig()]);
   else
      config=modifyConfig(customConfig);

   for(i=0;i<2;++i)
      ai[i]=new Ai(i,ship,bullets,mines,&config);
}

void MyMainView::writeConfig()
{
   KConfigGroup game(KSharedConfig::openConfig(), "Game");

   game.writeEntry("gravity",customConfig.gravity);
   game.writeEntry("acceleration",customConfig.acc);
   game.writeEntry("bulletDamage",customConfig.bulletDamage);
   game.writeEntry("bulletLifeTime",customConfig.bulletLifeTime);
   game.writeEntry("bulletReloadTime",customConfig.bulletReloadTime);
   game.writeEntry("mineDamage",customConfig.mineDamage);
   game.writeEntry("shipDamage",customConfig.shipDamage);
   game.writeEntry("maxBullets",customConfig.maxBullets);
   game.writeEntry("maxMines",customConfig.maxMines);
   game.writeEntry("rotationSpeed",customConfig.rotationSpeed);
   game.writeEntry("shotSpeed",customConfig.shotSpeed);
   game.writeEntry("accEnergyNeed",customConfig.energyNeed);
   game.writeEntry("rotationEnergyNeed",customConfig.rotationEnergyNeed);
   game.writeEntry("sunEnergy",customConfig.sunEnergy);
   game.writeEntry("mineActivateTime",customConfig.mineActivateTime);
   game.writeEntry("mineReloadTime",customConfig.mineReloadTime);
   game.writeEntry("mineFuel",customConfig.mineFuel);
   game.writeEntry("shotEnergyNeed",customConfig.shotEnergyNeed);
   game.writeEntry("mineEnergyNeed",customConfig.mineEnergyNeed);

   game.writeEntry("startPosX",customConfig.startPosX);
   game.writeEntry("startPosY",customConfig.startPosY);
   game.writeEntry("startVelX",customConfig.startVelX);
   game.writeEntry("startVelY",customConfig.startVelY);

   game.writeEntry("powerupLifeTime",customConfig.powerupLifeTime);
   game.writeEntry("powerupRefreshTime",customConfig.powerupRefreshTime);
   game.writeEntry("powerupShieldAmount",customConfig.powerupShieldAmount);
   game.writeEntry("powerupEnergyAmount",customConfig.powerupEnergyAmount);
}

SConfig MyMainView::modifyConfig(const SConfig &conf)
{
   SConfig newConfig=conf;
   newConfig.gamespeed*=Options::refreshTime()/33.0;
   newConfig.acc*=newConfig.gamespeed;
   newConfig.rotationSpeed*=newConfig.gamespeed*M_PI/ROTNUM*4;
   newConfig.energyNeed*=newConfig.gamespeed;
   newConfig.rotationEnergyNeed*=newConfig.gamespeed;
   newConfig.mineActivateTime*=newConfig.gamespeed;

   return newConfig;
}

void MyMainView::keyPressEvent(QKeyEvent *ev)
{
    // if-statement kept for historical reasons, maybe not needed anymore
    if ( ((gameEnd>0.0) || (gameEnd<=-2.0)) && (!waitForStart) )
    {
      if(actionCollection->action(QStringLiteral( "P1KeyLeft" ))->shortcuts().contains(ev->key()))
            playerKeyPressed[0][PlayerKeyLeft]=true;
      else if(actionCollection->action(QStringLiteral( "P2KeyLeft" ))->shortcuts().contains(ev->key()))
            playerKeyPressed[1][PlayerKeyLeft]=true;

      else if(actionCollection->action(QStringLiteral( "P1KeyRight" ))->shortcuts().contains(ev->key()))
            playerKeyPressed[0][PlayerKeyRight]=true;
      else if(actionCollection->action(QStringLiteral( "P2KeyRight" ))->shortcuts().contains(ev->key()))
            playerKeyPressed[1][PlayerKeyRight]=true;

      else if(actionCollection->action(QStringLiteral( "P1KeyAcc" ))->shortcuts().contains(ev->key()))
            playerKeyPressed[0][PlayerKeyAcc]=true;
      else if(actionCollection->action(QStringLiteral( "P2KeyAcc" ))->shortcuts().contains(ev->key()))
            playerKeyPressed[1][PlayerKeyAcc]=true;

      else if(actionCollection->action(QStringLiteral( "P1Shot" ))->shortcuts().contains(ev->key()))
            playerKeyPressed[0][PlayerKeyShot]=true;
      else if(actionCollection->action(QStringLiteral( "P2Shot" ))->shortcuts().contains(ev->key()))
            playerKeyPressed[1][PlayerKeyShot]=true;

      else if(actionCollection->action(QStringLiteral( "P1Mine" ))->shortcuts().contains(ev->key()))
            playerKeyPressed[0][PlayerKeyMine]=true;
      else if(actionCollection->action(QStringLiteral( "P2Mine" ))->shortcuts().contains(ev->key()))
            playerKeyPressed[1][PlayerKeyMine]=true;
      else
        ev->ignore();
    }
}

void MyMainView::keyReleaseEvent(QKeyEvent *ev)
{
   if(actionCollection->action(QStringLiteral( "P1KeyLeft" ))->shortcuts().contains(ev->key()))
      playerKeyPressed[0][PlayerKeyLeft]=false;
   else if(actionCollection->action(QStringLiteral( "P2KeyLeft" ))->shortcuts().contains(ev->key()))
      playerKeyPressed[1][PlayerKeyLeft]=false;

   else if(actionCollection->action(QStringLiteral( "P1KeyRight" ))->shortcuts().contains(ev->key()))
      playerKeyPressed[0][PlayerKeyRight]=false;
   else if(actionCollection->action(QStringLiteral( "P2KeyRight" ))->shortcuts().contains(ev->key()))
      playerKeyPressed[1][PlayerKeyRight]=false;

   else if(actionCollection->action(QStringLiteral( "P1KeyAcc" ))->shortcuts().contains(ev->key()))
      playerKeyPressed[0][PlayerKeyAcc]=false;
   else if(actionCollection->action(QStringLiteral( "P2KeyAcc" ))->shortcuts().contains(ev->key()))
      playerKeyPressed[1][PlayerKeyAcc]=false;

   else if(actionCollection->action(QStringLiteral( "P1Shot" ))->shortcuts().contains(ev->key()))
      playerKeyPressed[0][PlayerKeyShot]=false;
   else if(actionCollection->action(QStringLiteral( "P2Shot" ))->shortcuts().contains(ev->key()))
      playerKeyPressed[1][PlayerKeyShot]=false;

   else if(actionCollection->action(QStringLiteral( "P1Mine" ))->shortcuts().contains(ev->key()))
      playerKeyPressed[0][PlayerKeyMine]=false;
   else if(actionCollection->action(QStringLiteral( "P2Mine" ))->shortcuts().contains(ev->key()))
      playerKeyPressed[1][PlayerKeyMine]=false;
   else
      ev->ignore();
}

void MyMainView::pause()
{
   if( !waitForStart )
   {
      pauseAction->setChecked( true );

      waitForStart=true;
      QAbstractEventDispatcher::instance()->unregisterTimers(this);
      Q_EMIT setStatusText(i18n(" paused "), IDS_PAUSE);
   }
}

void MyMainView::resume()
{
   waitForStart=false;
   timerID=startTimer(Options::refreshTime());
   Q_EMIT setStatusText(QLatin1String( "" ),IDS_PAUSE);
   Q_EMIT setStatusText(QLatin1String( "" ),IDS_MAIN);
}

void MyMainView::start( )
{
   if( ( gameEnd <= 0.0 ) && ( gameEnd > -2.0 ) )
   {
      newRound( );
   }
   else if( waitForStart )
   {
      waitForStart = false;
      timerID=startTimer(Options::refreshTime());
      Q_EMIT setStatusText(QLatin1String( "" ),IDS_PAUSE);
      Q_EMIT setStatusText(QLatin1String( "" ),IDS_MAIN);
      pauseAction->setEnabled( true );
      pauseAction->setChecked( false );
   }
}

void MyMainView::stop()
{
   pauseAction->setEnabled( false );
   pauseAction->setChecked( false );

   QAbstractEventDispatcher::instance()->unregisterTimers(this);
   waitForStart = true;
}

void MyMainView::togglePause( )
{
   if( waitForStart )
      resume( );
   else
      pause( );
}

void MyMainView::resizeEvent(QResizeEvent *event)
{
   double mx,my;
   int i,current;
   int listsize; // used for caching QtList::size()

   mx=(event->size().width()-event->oldSize().width())/2.0;
   my=(event->size().height()-event->oldSize().height())/2.0;
   QWidget::resizeEvent(event);
   view.resize(width(),height());
   field.setSceneRect(0, 0, width(),height());

   // printf("%d %d\n",field.width(),field.height());

   sun->setPos(QPointF(width()/2-1-(sun->width()/2),
                       height()/2-1-(sun->height()/2)));

   for(i=0;i<2;++i)
   {
      // ship[i]->adoptSpritefieldBounds();
      ship[i]->moveBy(mx,my);

      listsize = mines[i]->size();
      for (current=0; current<listsize; ++current)
      {
         // mine->adoptSpritefieldBounds();
         mines[i]->value(current)->moveBy(mx,my);
      }

      listsize = bullets[i]->size();
      for (current=0; current<listsize; ++current)
      {
         // bullet->adoptSpritefieldBounds();
         bullets[i]->value(current)->moveBy(mx,my);
      }
   }
   if(textSprite)
      textSprite->moveBy((int)mx,(int)my);

   listsize = powerups.size();
   for (current=0; current<listsize; ++current)
   {
      powerups[current]->moveBy(mx,my);
   }
}

void MyMainView::newRound()
{
   double mx,my;
   int i;

   timeToNextPowerup=random.bounded(config.powerupRefreshTime);
   qDeleteAll(powerups);
   powerups.clear();

   QAbstractEventDispatcher::instance()->unregisterTimers(this);
   mx=width()/2.0;
   my=height()/2.0;
   ship[0]->setPos(QPointF(mx+config.startPosX-(ship[0]->width()/2),
                           my+config.startPosY-(ship[0]->height()/2)));
   ship[0]->setRotation(0.0);

   ship[1]->setPos(QPointF(mx-config.startPosX-(ship[1]->width()/2),
                           my-config.startPosY-(ship[1]->height()/2)));
   ship[1]->setRotation(M_PI);

   ship[0]->setVelocity(config.startVelX,config.startVelY);
   ship[1]->setVelocity(-config.startVelX,-config.startVelY);
   for(i=0;i<2;++i)
   {
      ship[i]->show();
      ship[i]->setEnergy(MAX_ENERGY);
      ship[i]->setHitPoints(Options::startHitPoints(i));
      ship[i]->stop(false);
      ship[i]->setExplosion(-1);
      Q_EMIT energy(i,(int)ship[i]->getEnergy());
      Q_EMIT hitPoints(i,ship[i]->getHitPoints());
      bulletShot[i]=false;
      qDeleteAll(*bullets[i]);
      bullets[i]->clear();

      qDeleteAll(*mines[i]);
      mines[i]->clear();

      ship[i]->mine(0.0);
      ship[i]->bullet(0.0);
      ship[i]->setBulletPowerups(0);
      ship[i]->setMinePowerups(0);

      ai[i]->newRound();
   }
   qDeleteAll(explosions);
   explosions.clear();
   gameEnd=-10.0;
   for(i=0;i<PlayerKeyNum;++i)
   {
      playerKeyPressed[0][i]=false;
      playerKeyPressed[1][i]=false;
   }
   if(textSprite)
   {
      textSprite->hide();
      delete textSprite;
      textSprite = nullptr;
   }
   //field.update();

   QString str = i18n("Press %1 to start",
                  actionCollection->action(QStringLiteral("game_start"))->shortcut().toString(QKeySequence::NativeText));
   Q_EMIT setStatusText(str,IDS_MAIN);
   Q_EMIT setStatusText( QLatin1String( "" ), IDS_PAUSE );
   stop( );
}

void MyMainView::newGame()
{
   int i;
   for(i=0;i<2;i++)
   {
      ship[i]->setWins(0);
      Q_EMIT wins(i,0);
   }
   newRound();
}

void MyMainView::timerEvent(QTimerEvent *event)
{
   unsigned w;
   int i;
   bool stopped = false;

   if(event->timerId()==timerID)
   {
      QAbstractEventDispatcher::instance()->unregisterTimers(this);
      if(gameEnd>0.0)
      {
         gameEnd-=1.0;
         if(gameEnd<=0.0)
         {
            stopped = true;
            if(textSprite)
            {
               textSprite->hide();
               delete textSprite;
               textSprite = nullptr;
            }
            textSprite = new QGraphicsSimpleTextItem(nullptr);
            field.addItem(textSprite);
	    // FIXME
           // textSprite->setTextFlags(Qt::AlignCenter);
	    textSprite->setBrush(QBrush(QColor(255,160,0)));
            textSprite->setFont(QFont(QFontDatabase::systemFont(QFontDatabase::GeneralFont).family(),14));
            textSprite->show( );
            if(ship[0]->getHitPoints()==0)
            {
               if(ship[1]->getHitPoints()==0)
                  textSprite->setText(i18n("draw round"));
               else
               {
                  textSprite->setText(i18n("blue player won the round"));
                  w=ship[1]->getWins()+1;
                  ship[1]->setWins(w);
                  Q_EMIT wins(1,w);
               }
            }
            else
            {
               textSprite->setText(i18n("red player won the round"));
               w=ship[0]->getWins()+1;
               ship[0]->setWins(w);
               Q_EMIT wins(0,w);
            }
            // must do this after setting text, because length is unknown until now
            textSprite->setPos(QPointF((width()-textSprite->boundingRect().width()) / 2,height()/2-90));

            QString str = i18n("Press %1 for new round",
                          actionCollection->action(QStringLiteral("game_start"))->shortcut().toString(QKeySequence::NativeText));
            Q_EMIT setStatusText(str,IDS_MAIN);
            stop( );
         }
      }

      if( !stopped )
      {
         for(i=0;i<2;++i)
            if(Options::playerIsAi(i)&&(ship[i]->getHitPoints()>0))
               ai[i]->think();

         moveMines();
         moveBullets();
         moveExplosions();
         moveShips();
         calculatePowerups();
         collisions();
         timerID=startTimer(Options::refreshTime());
      }
     //field.update();
   }
}

void MyMainView::moveShips()
{
   int i,olde;
   double nx,ny,en,nr;
   BulletSprite *bullet;
   MineSprite *mine;


   for(i=0;i<2;++i)
   {
      bool playerIsAi = Options::playerIsAi(i);
      olde=(int)ship[i]->getEnergy();
      if(ship[i]->getHitPoints()==0)
      {
         ship[i]->forward(config.gamespeed);
         ship[i]->calculateGravityAndEnergy(config.gravity,config.sunEnergy,
                                            config.gamespeed);
      }
      else
      {
         ship[i]->calculateGravityAndEnergy(config.gravity,config.sunEnergy,
                                         config.gamespeed);


         if ((!playerIsAi && playerKeyPressed[i][PlayerKeyRight]) ||
             (playerIsAi && ai[i]->rotateRight()))
            ship[i]->rotateRight(config.rotationEnergyNeed,
                                 config.rotationSpeed);

         if ((!playerIsAi && playerKeyPressed[i][PlayerKeyLeft]) ||
             (playerIsAi && ai[i]->rotateLeft()))
            ship[i]->rotateLeft(config.rotationEnergyNeed,
                                config.rotationSpeed);

         en=ship[i]->getEnergy();
         nr=ship[i]->getRotation();

         nx=cos(nr);
         ny=sin(nr);
         if (((!playerIsAi && playerKeyPressed[i][PlayerKeyAcc]) ||
              (playerIsAi && ai[i]->accelerate()))
	     && (en>config.energyNeed) )
         {
            en-=config.energyNeed;
            ship[i]->setVelocity(ship[i]->xVelocity()+nx*config.acc,
                                 ship[i]->yVelocity()-ny*config.acc);

	    // limit speed to avoid "tunneling" through other objects
	    // FIXME: find a more elegant way
	    if (ship[i]->xVelocity()*ship[i]->xVelocity()+
		ship[i]->yVelocity()*ship[i]->yVelocity() > MAX_VELOCITY*MAX_VELOCITY)
	    {
		    double alpha;
		    alpha = fabs(atan(ship[i]->yVelocity()/ship[i]->xVelocity()));
		    ship[i]->setVelocity(MAX_VELOCITY*cos(alpha)*fabs(ship[i]->xVelocity())/ship[i]->xVelocity(),
                                 MAX_VELOCITY*sin(alpha)*fabs(ship[i]->yVelocity())/ship[i]->yVelocity());
	    }
         }
         if(en>MAX_ENERGY)
            en=MAX_ENERGY;

         ship[i]->forward(config.gamespeed);

             //Bullets and Mines
         if ((!playerIsAi && playerKeyPressed[i][PlayerKeyShot]) ||
             (playerIsAi && ai[i]->shootBullet()))
         {
            if((en>config.shotEnergyNeed) && (!ship[i]->reloadsBullet()))
            {
               if(bullets[i]->count() <
                  (static_cast<int>(config.maxBullets) + ship[i]->getBulletPowerups()))
               {
                  ship[i]->bullet(config.bulletReloadTime);
                  en-=config.shotEnergyNeed;
		  if(i)
		     bullet=new BulletSprite(svgrender, QStringLiteral( MV_BULLET2 ), i,
					     config.bulletLifeTime);
		  else
		     bullet=new BulletSprite(svgrender, QStringLiteral( MV_BULLET1 ), i,
                                          config.bulletLifeTime);
		  field.addItem(bullet);
		  QPointF p;
		  p = ship[i]->mapToScene(ship[i]->center());
		  bullet->setPos(QPointF(p.x()+nx*SHOTDIST,p.y()-ny*SHOTDIST));
                  bullet->setVelocity(ship[i]->xVelocity()+nx*config.shotSpeed,
                                      ship[i]->yVelocity()-ny*config.shotSpeed);
                  // bullet->setBoundsAction(QwRealMobileSprite::Wrap);
                  bullet->show();
                  bullets[i]->append(bullet);
               }
            }
         }
         if ((!Options::playerIsAi(i) && playerKeyPressed[i][PlayerKeyMine]) ||
             (Options::playerIsAi(i) && ai[i]->layMine()))
         {
            if((en>config.mineEnergyNeed) && (!ship[i]->reloadsMine()))
            {
               if(mines[i]->count() <
                  (static_cast<int>(config.maxMines) + ship[i]->getMinePowerups()))
               {
                  ship[i]->mine(config.mineReloadTime);
                  en-=config.mineEnergyNeed;
		  if (i==0)
                     mine=new MineSprite(svgrender,animation[ID_MINE1],animation[ID_MINEEXPLO],i,
                                      config.mineActivateTime,config.mineFuel);
		  else
		     mine=new MineSprite(svgrender,animation[ID_MINE2],animation[ID_MINEEXPLO],i,
                                      config.mineActivateTime,config.mineFuel);
		  field.addItem(mine);
		  mine->setPos(ship[i]->mapToScene(ship[i]->center()));
                  // move mine to center
                  mine->moveBy(-mine->center().x(),-mine->center().y());
                  mine->setVelocity(0,0);
                  //mine->setBoundsAction(QwRealMobileSprite::Wrap);
                  mine->show();
                  mines[i]->append(mine);
               }
            }
         }
         ship[i]->setEnergy(en);
         if(olde!=(int)en)
            Q_EMIT energy(i,(int)en);
      }
   }
}

void MyMainView::moveMines()
{
   int i;
   MineSprite* mine;
   int p;
   int listsize; // used for caching QtList::size()

   for(p=0;p<2;p++)
   {
      i=0;
      listsize = mines[p]->size();
      while (i<listsize)
      {
         mine = mines[p]->value(i);
         mine->calculateGravity(config.gravity,config.gamespeed);
         mine->forward(config.gamespeed);
         if(mine->over())
         {
            mine->hide();
            mines[p]->removeAt(i);
            delete mine;
            listsize--;
         }
         else
            i++;
      }
   }
}

void MyMainView::moveBullets()
{
   int i,j;
   BulletSprite *sp;
   int listsize; // used for caching QtList::size()

   for(i=0;i<2;i++)
   {
      j=0;
      listsize = bullets[i]->size();
      while (j<listsize)
      {
         sp = bullets[i]->value(j);
         sp->calculateGravity(config.gravity,config.gamespeed);
         sp->forward(config.gamespeed);
         if(sp->timeOut())
         {
            sp->hide();
            bullets[i]->removeAll(sp);
            listsize--;
         }
         else
            j++;
      }
   }
}

void MyMainView::moveExplosions()
{
   int i=0;
   ExplosionSprite *ex;
   int listsize; // used for caching QtList::size()
   listsize = explosions.size();
   while (i<listsize)
   {
      ex = explosions[i];
      ex->forward(config.gamespeed);
      if(ex->isOver())
      {
         explosions.removeAt(i);
         delete ex;
         listsize--;
      }
      else
         i++;
   }
}

void MyMainView::calculatePowerups()
{
   int i=0;
   PowerupSprite *sp;
   int listsize; // used for caching QtList::size()
   listsize = powerups.size();
   while (i<listsize)
   {
      sp = powerups[i];
      sp->setLifetime(sp->getLifetime()-config.gamespeed);
      if (sp->getLifetime()<0)
      {
         powerups.removeAt(i);
         delete sp;
         listsize--;
      }
      else
         i++;
   }
   timeToNextPowerup-=config.gamespeed;
   if(timeToNextPowerup<0)
   {
      int type,x,y;
      timeToNextPowerup= random.bounded(config.powerupRefreshTime);
      type= random.bounded(PowerupSprite::PowerupNum);
      sp=new PowerupSprite(svgrender,powerupelements[type],type,
                           config.powerupLifeTime);
      field.addItem(sp);
      do
      {
         x = random.bounded(width()-40)+20;
         y = random.bounded(height()-40)+20;
      }
      while(((x-width()/2)*(x-width()/2)+(y-height()/2)*(y-height()/2))<(50*50));
      sp->setPos(QPointF(x,y));
      powerups.append(sp);
      sp->show();
   }
}

void MyMainView::collisions()
{
   int pl,hp,oldhp[2],ohp;
   QList<QGraphicsItem *> unexact;
   BulletSprite *bullet;
   MineSprite *mine;
   ExplosionSprite *expl;
   ShipSprite *s;
   PowerupSprite *power;
   QList<QGraphicsItem *> hitlist;
   double ndx[2],ndy[2];
   double en;
   int i;
   int listsize; // used for caching QtList::size()

   for(pl=0;pl<2;++pl)
   {
      if(!ship[pl]->isStopped())
      {
         unexact.clear();
	 unexact = ship[pl]->collidingItems(Qt::IntersectsItemBoundingRect);
         oldhp[pl]=hp=ship[pl]->getHitPoints();
         hitlist.clear();
	 for (QGraphicsItem *sprite : qAsConst(unexact)) {
            if((sprite->type()!=S_EXPLOSION)
               && !((sprite->type()!=S_SUN)&&(ship[pl]->getHitPoints()==0)))
               if(ship[pl]->collidesWithItem(sprite,Qt::IntersectsItemShape))
                  if(!hitlist.contains(sprite))
                     hitlist.append(sprite);
         }

         for (QGraphicsItem *sprite : qAsConst(hitlist)) {
            switch(sprite->type())
            {
               case S_SUN:
                  hp=0;
                  ship[pl]->stop();
                  break;
               case S_BULLET:
                  bullet=(BulletSprite *)sprite;
                  bullet->hide();
                  //bullets[bullet->getPlayerNumber()]->removeRef(bullet);
                  bullets[bullet->getPlayerNumber()]->removeAll(bullet);
                  delete bullet;
                  hp-=config.bulletDamage;
                  break;
               case S_SHIP:
                  s=(ShipSprite*)sprite;
                  ohp=s->getHitPoints();
                  if(ohp>0)
                  {
                     s->setHitPoints(ohp-hp-config.shipDamage);
                     Q_EMIT hitPoints(s->getPlayerNumber(),s->getHitPoints());
                     ndx[0]=((1-EPSILON)*ship[0]->xVelocity()+(1+EPSILON)*ship[1]->xVelocity())/2.0;
                     ndy[0]=((1-EPSILON)*ship[0]->yVelocity()+(1+EPSILON)*ship[1]->yVelocity())/2.0;
                     ndx[1]=((1-EPSILON)*ship[1]->xVelocity()+(1+EPSILON)*ship[0]->xVelocity())/2.0;
                     ndy[1]=((1-EPSILON)*ship[1]->yVelocity()+(1+EPSILON)*ship[0]->yVelocity())/2.0;
                     ship[0]->setVelocity(ndx[0],ndy[0]);
                     ship[1]->setVelocity(ndx[1],ndy[1]);
                     hp-=ohp+config.shipDamage;
                  }
                  break;
               case S_MINE:
                  mine=(MineSprite *)sprite;
                  if(mine->isActive()&& !mine->explodes())
                  {
                     mine->explode();
                     ndx[0]=(ship[pl]->xVelocity()+0.3*mine->xVelocity())/1.3;
                     ndy[0]=(ship[pl]->yVelocity()+0.3*mine->yVelocity())/1.3;
                     ship[pl]->setVelocity(ndx[0],ndy[0]);
                     mine->setVelocity(ndx[0],ndy[0]);
                     hp-=config.mineDamage;
                  }
                  break;
               case S_POWERUP:
                  power=(PowerupSprite *)sprite;
                  switch(power->getType())
                  {
                     case PowerupSprite::PowerupShield:
                        hp+=config.powerupShieldAmount;
                        break;
                     case PowerupSprite::PowerupEnergy:
                        en=ship[pl]->getEnergy()+config.powerupEnergyAmount;
                        if(en>MAX_ENERGY)
                           en=MAX_ENERGY;
                        ship[pl]->setEnergy(en);
                        break;
                     case PowerupSprite::PowerupMine:
                        ship[pl]->setMinePowerups(
                           ship[pl]->getMinePowerups()+1);
                        break;
                     case PowerupSprite::PowerupBullet:
                        ship[pl]->setBulletPowerups(
                           ship[pl]->getMinePowerups()+1);
                        break;
                  }
                  power->hide();
                  powerups.removeAll(power);
                  delete power;
                  break;
            }
         }
         if(hp>MAX_HP)
            hp=MAX_HP;
         ship[pl]->setHitPoints(hp);
      }

      listsize = mines[pl]->size();
      for (i=0; i<listsize; ++i)
      {
         mine = mines[pl]->value(i);
         if(!mine->explodes())
         {
            unexact.clear();
            unexact=mine->collidingItems(Qt::IntersectsItemBoundingRect);
            hitlist.clear();
	    for (QGraphicsItem *sprite : qAsConst(unexact)) {
               if(sprite->type()==S_BULLET)
                  if(mine->collidesWithItem(sprite))
                     if(!hitlist.contains(sprite))
                        hitlist.append(sprite);
            }
            if(!hitlist.isEmpty())
            {
               mine->explode();
	       for (QGraphicsItem *item : qAsConst(hitlist)) {
                  // FIXME: why does it crash with qgraphicsitem_cast?
		  bullet = static_cast<BulletSprite*>(item);// qgraphicsitem_cast<BulletSprite*>(item);
//                   bullets[bullet->getPlayerNumber()]->removeRef(bullet);
                  bullets[bullet->getPlayerNumber()]->removeAll(bullet);
                  delete bullet;
               }
            }
         }
      }
   }

   hitlist.clear();
   unexact.clear();
   unexact=sun->collidingItems(Qt::IntersectsItemBoundingRect);
   for (QGraphicsItem *sprite : qAsConst(unexact)) {
      switch(sprite->type())
      {
         case S_BULLET:
            if(sun->collidesWithItem(sprite))
               if(!hitlist.contains(sprite))
                  hitlist.append(sprite);
            break;
         case S_MINE:
            if(!((MobileSprite*)sprite)->isStopped())
               if(sun->collidesWithItem(sprite))
                  if(!hitlist.contains(sprite))
                     hitlist.append(sprite);
            break;
      }
   }

   for (QGraphicsItem *sprite : qAsConst(hitlist)) {
      switch(sprite->type())
      {
         case S_BULLET:
            bullet=(BulletSprite *)sprite;
            bullet->hide();
            bullets[bullet->getPlayerNumber()]->removeAll(bullet);
            delete bullet;
            break;
         case S_MINE:
            mine=(MineSprite*)sprite;
            mine->stop();
            if(!mine->explodes())
               mine->explode();
            break;
      }
   }


   for(pl=0;pl<2;++pl)
   {
      hp=ship[pl]->getHitPoints();
      if(hp!=oldhp[pl])
         Q_EMIT hitPoints(pl,hp);
      if((hp==0)&&(ship[pl]->getExplosion()<0))
      {
         ship[pl]->setExplosion((int)(EXPLOSION_TIME/config.gamespeed));
	 expl = new ExplosionSprite(svgrender,animation[ID_EXPLOSION],ship[pl]);
	 field.addItem(expl);
         expl->show();
         explosions.append(expl);
         gameEnd=Options::timeAfterKill()/config.gamespeed;
      }
   }
}

void MyMainView::gameSetup()
{
  if(!waitForStart)
    pause();

  if (KConfigDialog::showDialog( QStringLiteral( "settings" )))
    return;

  SettingsDialog *settings=new SettingsDialog(&customConfig,this,"settings");
  connect(settings, &SettingsDialog::settingsUpdated, this, &MyMainView::closeSettings);
  settings->show();
}

void MyMainView::closeSettings(){
  if(Options::lastConfig()<predefinedConfigNum)
    config=modifyConfig(predefinedConfig[Options::lastConfig()]);
  else
    config=modifyConfig(customConfig);
}

void MyMainView::focusOutEvent (QFocusEvent * /*event*/)
{
   /* FIXME: ugly hack, I think
      it's maybe better to declare something like QMyScene:public QGraphicsScene and process input there */
   setFocus(Qt::OtherFocusReason);
}


