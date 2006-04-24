#include "mainview.h"

#include <math.h>
#include <qbitmap.h>
#include <qfile.h>
#include <q3vbox.h>
//Added by qt3to4:
#include <QPixmap>
#include <Q3PtrList>
#include <QKeyEvent>
#include <QTimerEvent>
#include <QResizeEvent>
#include <QAbstractEventDispatcher>
#include <kapplication.h>
#include <kaction.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <kglobalsettings.h>
#include <kconfig.h>
#include <kiconloader.h>
#include <kglobal.h>

#include "ai.h"
#include "options.h"

KToggleAction *MyMainView::pauseAction = 0;

MyMainView::MyMainView(QWidget *parent)
    :QWidget(parent),
    field(DEF_WIDTH,DEF_HEIGHT),
    view(&field,this)
{
   int i,p;
   setMinimumSize(600,400);
   random.setSeed(0);
   QPixmap backgr(locate("appdata", MV_BACKGROUND));
   field.setBackgroundPixmap(backgr);

   view.setResizePolicy(Q3ScrollView::AutoOne);
   view.setHScrollBarMode(Q3ScrollView::AlwaysOff);
   view.setVScrollBarMode(Q3ScrollView::AlwaysOff);

   for(p=0;p<2;p++)
   {
      for(i=0;i<PlayerKeyNum;i++)
         playerKeyPressed[p][i]=false;
      bulletShot[p]=false;
      minePut[p]=false;
   }

   QString tmp = KGlobal::dirs()->findResourceDir("appdata", (QString)MV_BACKGROUND);

   Q3CanvasPixmapArray *sunsequence
      = loadOldPixmapSequence( tmp + MV_SUN_PPM, tmp + MV_SUN_PBM );
   sun=new SunSprite(sunsequence, &field);
   sun->move(width()/2-1,height()/2-1);
   sun->show();

   explosionsequence = loadOldPixmapSequence( tmp + MV_EXPLOSION_PPM,
                                              tmp + MV_EXPLOSION_PBM, 31 );
   mineexplosionsequence = loadOldPixmapSequence(tmp + MV_MINEEX_PPM,
                                                 tmp + MV_MINEEX_PBM, 18 );

   shipsequence[0] = loadOldPixmapSequence( tmp + MV_SHIP1_PPM,
                                            tmp + MV_SHIP1_PBM, ROTNUM );
   shipsequence[1] = loadOldPixmapSequence( tmp + MV_SHIP2_PPM,
                                            tmp + MV_SHIP2_PBM, ROTNUM);
   ship[0]=new ShipSprite(shipsequence[0],&field,0);
   ship[1]=new ShipSprite(shipsequence[1],&field,1);

   bulletsequence[0] = loadOldPixmapSequence( tmp + MV_BULLET1_PPM,
                                              tmp + MV_BULLET1_PBM );
   bulletsequence[1] = loadOldPixmapSequence( tmp + MV_BULLET2_PPM,
                                              tmp + MV_BULLET2_PBM );
   minesequence[0] = loadOldPixmapSequence( tmp + MV_MINE1_PPM,
                                            tmp + MV_MINE1_PBM, 2);
   minesequence[1] = loadOldPixmapSequence( tmp + MV_MINE2_PPM,
                                            tmp + MV_MINE2_PBM, 2);
   powerupsequence[PowerupSprite::PowerupMine]
      = loadOldPixmapSequence( tmp + MV_POWERMINE_PPM, tmp + MV_POWERMINE_PBM );
   powerupsequence[PowerupSprite::PowerupBullet]
      = loadOldPixmapSequence( tmp + MV_POWERBULLET_PPM, tmp + MV_POWERBULLET_PBM );
   powerupsequence[PowerupSprite::PowerupShield]
      = loadOldPixmapSequence( tmp + MV_POWERSHIELD_PPM, tmp + MV_POWERSHIELD_PBM );
   powerupsequence[PowerupSprite::PowerupEnergy]
      = loadOldPixmapSequence( tmp + MV_POWERENERGY_PPM, tmp + MV_POWERENERGY_PBM );

   for(i=0;i<2;i++)
   {
      // ship[i]->setBoundsAction(QwRealMobileSprite::Wrap);
      ship[i]->hide();
      bullets[i]=new Q3PtrList<BulletSprite>;
      bullets[i]->setAutoDelete(true);
      mines[i]=new Q3PtrList<MineSprite>;
      mines[i]->setAutoDelete(true);

   }

   explosions.setAutoDelete(true);
   powerups.setAutoDelete(true);

   waitForStart=false;
   textSprite=0;
   readConfig();
}

MyMainView::~MyMainView()
{
   QAbstractEventDispatcher::instance()->unregisterTimers(this);
   writeConfig();
}

void MyMainView::setActionCollection(KActionCollection *a)
{
   actionCollection = a;
}

void MyMainView::readConfig()
{
   KConfigGroup game(KGlobal::config(), "Game");
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

   for(i=0;i<2;i++)
      ai[i]=new Ai(i,ship,bullets,mines,&config);
}

void MyMainView::writeConfig()
{
   KConfigGroup game(KGlobal::config(), "Game");
    
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

SConfig MyMainView::modifyConfig(SConfig conf)
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
   if((gameEnd<=0.0)&&(gameEnd>-2.0))
   {
      /*
      if(key==options.functionKey[FunctionKeyStart])
         newRound();
      */
   }
   else if(waitForStart)
   {
      /*
      if((key==options.functionKey[FunctionKeyStart])
         && (!functionKeyPressed[FunctionKeyStart]))
      {
         functionKeyPressed[FunctionKeyStart]=true;
         resume();
      }
      */
   }
   else
   {
     bool accept=true;

#warning KKey does no longer exist
#if 0
      KKey key(ev);

      if(actionCollection->action("P1KeyLeft")->shortcut().contains(key))
            playerKeyPressed[0][PlayerKeyLeft]=true;
      else if(actionCollection->action("P2KeyLeft")->shortcut().contains(key))
            playerKeyPressed[1][PlayerKeyLeft]=true;
      
      else if(actionCollection->action("P1KeyRight")->shortcut().contains(key))
            playerKeyPressed[0][PlayerKeyRight]=true;
      else if(actionCollection->action("P2KeyRight")->shortcut().contains(key))
            playerKeyPressed[1][PlayerKeyRight]=true;
      
      else if(actionCollection->action("P1KeyAcc")->shortcut().contains(key))
            playerKeyPressed[0][PlayerKeyAcc]=true;
      else if(actionCollection->action("P2KeyAcc")->shortcut().contains(key))
            playerKeyPressed[1][PlayerKeyAcc]=true;
      
      else if(actionCollection->action("P1Shot")->shortcut().contains(key))
            playerKeyPressed[0][PlayerKeyShot]=true;
      else if(actionCollection->action("P2Shot")->shortcut().contains(key))
            playerKeyPressed[1][PlayerKeyShot]=true;
      
      else if(actionCollection->action("P1Mine")->shortcut().contains(key))
            playerKeyPressed[0][PlayerKeyMine]=true;
      else if(actionCollection->action("P2Mine")->shortcut().contains(key))
            playerKeyPressed[1][PlayerKeyMine]=true;
      else
        accept = false;
      /*
      if((key==options.functionKey[FunctionKeyStart])
         && (!functionKeyPressed[FunctionKeyStart]))
      {
         functionKeyPressed[FunctionKeyStart]=true;
         pause();
      }
      */
#endif
      if(!accept)
	ev->ignore();
   }
}

void MyMainView::keyReleaseEvent(QKeyEvent *ev)
{
   bool accept=true;
#warning KKey does no longer exist
#if 0
   KKey key(ev);

   if(actionCollection->action("P1KeyLeft")->shortcut().contains(key))
      playerKeyPressed[0][PlayerKeyLeft]=false;
   else if(actionCollection->action("P2KeyLeft")->shortcut().contains(key))
      playerKeyPressed[1][PlayerKeyLeft]=false;
      
   else if(actionCollection->action("P1KeyRight")->shortcut().contains(key))
      playerKeyPressed[0][PlayerKeyRight]=false;
   else if(actionCollection->action("P2KeyRight")->shortcut().contains(key))
      playerKeyPressed[1][PlayerKeyRight]=false;
      
   else if(actionCollection->action("P1KeyAcc")->shortcut().contains(key))
      playerKeyPressed[0][PlayerKeyAcc]=false;
   else if(actionCollection->action("P2KeyAcc")->shortcut().contains(key))
      playerKeyPressed[1][PlayerKeyAcc]=false;
      
   else if(actionCollection->action("P1Shot")->shortcut().contains(key))
      playerKeyPressed[0][PlayerKeyShot]=false;
   else if(actionCollection->action("P2Shot")->shortcut().contains(key))
      playerKeyPressed[1][PlayerKeyShot]=false;
      
   else if(actionCollection->action("P1Mine")->shortcut().contains(key))
      playerKeyPressed[0][PlayerKeyMine]=false;
   else if(actionCollection->action("P2Mine")->shortcut().contains(key))
      playerKeyPressed[1][PlayerKeyMine]=false;
   else
      accept = false;

#endif
   if(!accept)
     ev->ignore();
}

void MyMainView::pause()
{
   if( !waitForStart )
   {
      pauseAction->setChecked( true );

      waitForStart=true;
      QAbstractEventDispatcher::instance()->unregisterTimers(this);
      emit setStatusText(i18n(" paused "), IDS_PAUSE);
   }
}

void MyMainView::resume()
{
   waitForStart=false;
   timerID=startTimer(Options::refreshTime());
   emit(setStatusText("",IDS_PAUSE));
   emit(setStatusText("",IDS_MAIN));
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
      emit(setStatusText("",IDS_PAUSE));
      emit(setStatusText("",IDS_MAIN));
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
   MineSprite *mine;
   BulletSprite *bullet;
   PowerupSprite *powerup;
   int i,current;

   mx=(event->size().width()-event->oldSize().width())/2.0;
   my=(event->size().height()-event->oldSize().height())/2.0;
   QWidget::resizeEvent(event);
   view.resize(width(),height());
   field.resize(width(),height());

   // printf("%d %d\n",field.width(),field.height());
   sun->move(width()/2-1,height()/2-1);

   for(i=0;i<2;i++)
   {
      // ship[i]->adoptSpritefieldBounds();
      ship[i]->moveBy(mx,my);
      current=mines[i]->at();
      for(mine=mines[i]->first();mine;mine=mines[i]->next())
      {
         // mine->adoptSpritefieldBounds();
         mine->moveBy(mx,my);
      }
      if(current>=0)
         mines[i]->at(current);

      current=bullets[i]->at();
      for(bullet=bullets[i]->first();bullet;bullet=bullets[i]->next())
      {
         // bullet->adoptSpritefieldBounds();
         bullet->moveBy(mx,my);
      }
      if(current>=0)
         bullets[i]->at(current);

   }
   if(textSprite)
      textSprite->moveBy((int)mx,(int)my);
   current=powerups.at();
   for(powerup=powerups.first();powerup;powerup=powerups.next())
      powerup->moveBy(mx,my);
   if(current>=0)
      powerups.at(current);
}

void MyMainView::newRound()
{
   double mx,my;
   int i;

   timeToNextPowerup=random.getDouble() * config.powerupRefreshTime;
   powerups.clear();

   QAbstractEventDispatcher::instance()->unregisterTimers(this);
   mx=width()/2.0;
   my=height()/2.0;
   ship[0]->move(mx+config.startPosX,my+config.startPosY);
   ship[0]->setRotation(0.0);
   ship[0]->setFrame(0);

   ship[1]->move(mx-config.startPosX,my-config.startPosY);
   ship[1]->setRotation(M_PI);
   ship[1]->setFrame(ROTNUM/2);

   ship[0]->setVelocity(config.startVelX,config.startVelY);
   ship[1]->setVelocity(-config.startVelX,-config.startVelY);
   for(i=0;i<2;i++)
   {
      ship[i]->show();
      ship[i]->setEnergy(99.9);
      ship[i]->setHitPoints(Options::startHitPoints(i));
      ship[i]->stop(false);
      ship[i]->setExplosion(-1);
      emit(energy(i,(int)ship[i]->getEnergy()));
      emit(hitPoints(i,ship[i]->getHitPoints()));
      bulletShot[i]=false;
      bullets[i]->clear();
      mines[i]->clear();
      ship[i]->mine(0.0);
      ship[i]->bullet(0.0);
      ship[i]->setBulletPowerups(0);
      ship[i]->setMinePowerups(0);

      ai[i]->newRound();
   }
   explosions.clear();
   gameEnd=-10.0;
   for(i=0;i<PlayerKeyNum;i++)
   {
      playerKeyPressed[0][i]=false;
      playerKeyPressed[1][i]=false;
   }
   if(textSprite)
   {
      textSprite->hide();
      delete textSprite;
      textSprite=0;
   }
   field.update();

   QString str = i18n("Press %1 to start",
                  KShortcut(GAME_START_SHORTCUT).toString());
   emit(setStatusText(str,IDS_MAIN));
   emit( setStatusText( "", IDS_PAUSE ) );
   stop( );
}

void MyMainView::newGame()
{
   int i;
   for(i=0;i<2;i++)
   {
      ship[i]->setWins(0);
      emit(wins(i,0));
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
               textSprite=0;
            }

            textSprite=new Q3CanvasText(&field);
            textSprite->move(width()/2,height()/2-90);
            textSprite->setTextFlags(Qt::AlignCenter);
            textSprite->setColor(qRgb(255,160,0));
            textSprite->setFont(QFont(KGlobalSettings::generalFont().family(),14));
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
                  emit(wins(1,w));
               }
            }
            else
            {
               textSprite->setText(i18n("red player won the round"));
               w=ship[0]->getWins()+1;
               ship[0]->setWins(w);
               emit(wins(0,w));
            }
            QString str = i18n("Press %1 for new round",
                           KShortcut(GAME_START_SHORTCUT).toString());
            emit(setStatusText(str,IDS_MAIN));
            stop( );
         }
      }

      if( !stopped )
      {
         for(i=0;i<2;i++)
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
      field.update();
   }
}

void MyMainView::moveShips()
{
   int i,nf,olde;
   double nx,ny,en,nr;
   BulletSprite *bullet;
   MineSprite *mine;


   for(i=0;i<2;i++)
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


         if(!playerIsAi&&playerKeyPressed[i][PlayerKeyRight]
            || playerIsAi&&ai[i]->rotateRight())
            ship[i]->rotateRight(config.rotationEnergyNeed,
                                 config.rotationSpeed);

         if(!playerIsAi&&playerKeyPressed[i][PlayerKeyLeft]
            || playerIsAi&&ai[i]->rotateLeft())
            ship[i]->rotateLeft(config.rotationEnergyNeed,
                                config.rotationSpeed);

         en=ship[i]->getEnergy();
         nr=ship[i]->getRotation();


         nf=ship[i]->frame();
         nx=cos(nr);
         ny=sin(nr);
         if((!playerIsAi&&playerKeyPressed[i][PlayerKeyAcc]
             || playerIsAi&&ai[i]->accelerate())
            &&(en>config.energyNeed))
         {
            en-=config.energyNeed;
            ship[i]->setVelocity(ship[i]->xVelocity()+nx*config.acc,
                                 ship[i]->yVelocity()-ny*config.acc);
         }
         if(en>99.9)
            en=99.9;

         ship[i]->forward(config.gamespeed);
             //Bullets and Mines
         if(!playerIsAi&&playerKeyPressed[i][PlayerKeyShot]
            ||playerIsAi&&ai[i]->shootBullet())
         {
            if((en>config.shotEnergyNeed) && (!ship[i]->reloadsBullet()))
            {
               if(bullets[i]->count() <
                  (config.maxBullets+ship[i]->getBulletPowerups()))
               {
                  ship[i]->bullet(config.bulletReloadTime);
                  en-=config.shotEnergyNeed;
                  bullet=new BulletSprite(bulletsequence[i],&field,i,
                                          config.bulletLifeTime);
                  bullet->move(ship[i]->x()+nx*SHOTDIST,
                                 ship[i]->y()-ny*SHOTDIST);
                  bullet->setVelocity(ship[i]->xVelocity()+nx*config.shotSpeed,
                                      ship[i]->yVelocity()-ny*config.shotSpeed);
                  // bullet->setBoundsAction(QwRealMobileSprite::Wrap);
                  bullet->show();

                  bullets[i]->append(bullet);
               }
            }
         }
         if(!Options::playerIsAi(i)&&playerKeyPressed[i][PlayerKeyMine]
            || Options::playerIsAi(i)&&ai[i]->layMine())
         {
            if((en>config.mineEnergyNeed) && (!ship[i]->reloadsMine()))
            {
               if(mines[i]->count() <
                  (config.maxMines+ship[i]->getMinePowerups()))
               {
                  ship[i]->mine(config.mineReloadTime);
                  en-=config.mineEnergyNeed;
                  mine=new MineSprite(minesequence[i],&field,i,
                                      config.mineActivateTime,config.mineFuel);
                  mine->move(ship[i]->x(),ship[i]->y());
                  mine->setVelocity(0,0);
                  //mine->setBoundsAction(QwRealMobileSprite::Wrap);
                  mine->show();
                  mines[i]->append(mine);
               }
            }
         }
         ship[i]->setEnergy(en);
         if(olde!=(int)en)
            emit(energy(i,(int)en));
      }
   }
}

void MyMainView::moveMines()
{
   MineSprite* mine;
   int p;

   for(p=0;p<2;p++)
   {
      mine=mines[p]->first();
      while(mine)
      {
         mine->calculateGravity(config.gravity,config.gamespeed);
         mine->forward(config.gamespeed);
         if(mine->over())
         {
            mine->hide();
            mines[p]->remove();
            mine=mines[p]->current();
         }
         else
            mine=mines[p]->next();
      }
   }
}

void MyMainView::moveBullets()
{
   int i;
   BulletSprite *sp;

   for(i=0;i<2;i++)
   {
      sp=bullets[i]->first();
      while(sp)
      {
         sp->calculateGravity(config.gravity,config.gamespeed);
         sp->forward(config.gamespeed);
         if(sp->timeOut())
         {
            sp->hide();
            bullets[i]->removeRef(sp);
            sp=bullets[i]->current();
         }
         else
            sp=bullets[i]->next();
      }
   }
}

void MyMainView::moveExplosions()
{
   ExplosionSprite *ex;
   ex=explosions.first();
   while(ex)
   {
      ex->forward(config.gamespeed);
      if(ex->isOver())
      {
         explosions.removeRef(ex);
         ex=explosions.current();
      }
      else
         ex=explosions.next();
   }
}

void MyMainView::calculatePowerups()
{
   PowerupSprite *sp;
   int type,x,y;

   sp=powerups.first();
   while(sp)
   {
      sp->setLifetime(sp->getLifetime()-config.gamespeed);
      if(sp->getLifetime()<0)
      {
         powerups.removeRef(sp);
         sp=powerups.current();
      }
      else
         sp=powerups.next();
   }
   timeToNextPowerup-=config.gamespeed;
   if(timeToNextPowerup<0)
   {
      timeToNextPowerup= random.getDouble() * config.powerupRefreshTime;
      type= random.getLong(PowerupSprite::PowerupNum);
      sp=new PowerupSprite(powerupsequence[type],&field,type,
                           config.powerupLifeTime);
      do
      {
         x = random.getLong(width()-40)+20;
         y = random.getLong(height()-40)+20;
      }
      while(((x-width()/2)*(x-width()/2)+(y-height()/2)*(y-height()/2))<(50*50));
      sp->move(x,y);
      powerups.append(sp);
      sp->show();
   }
}

void MyMainView::collisions()
{
   int pl,hp,op,oldhp[2],ohp;
   Q3CanvasItemList unexact;
   Q3CanvasItem *sprite;
   BulletSprite *bullet;
   MineSprite *mine;
   ExplosionSprite *expl;
   ShipSprite *s;
   PowerupSprite *power;
   Q3CanvasItemList hitlist;
   double ndx[2],ndy[2];
   double en;
   Q3CanvasItemList::Iterator it;

   for(pl=0;pl<2;pl++)
   {
      if(!ship[pl]->isStopped())
      {
         unexact.clear();
         unexact=ship[pl]->collisions(false);
         oldhp[pl]=hp=ship[pl]->getHitPoints();
         hitlist.clear();
         for(it=unexact.begin(); it != unexact.end(); ++it)
         {
            sprite = (*it);
            if((sprite->rtti()!=S_EXPLOSION)
               && !((sprite->rtti()!=S_SUN)&&(ship[pl]->getHitPoints()==0)))
               if(ship[pl]->collidesWith(sprite))
                  if(!hitlist.contains(sprite))
                     hitlist.append(sprite);
         }

         for(it=hitlist.begin(); it != hitlist.end(); ++it)
         {
            sprite = (*it);
            switch(sprite->rtti())
            {
               case S_SUN:
                  hp=0;
                  ship[pl]->stop();
                  break;
               case S_BULLET:
                  bullet=(BulletSprite *)sprite;
                  bullet->hide();
                  bullets[bullet->getPlayerNumber()]->removeRef(bullet);
                  hp-=config.bulletDamage;
                  break;
               case S_SHIP:
                  s=(ShipSprite*)sprite;
                  ohp=s->getHitPoints();
                  if(ohp>0)
                  {
                     s->setHitPoints(ohp-hp-config.shipDamage);
                     emit(hitPoints(s->getPlayerNumber(),s->getHitPoints()));
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
                     mine->explode(mineexplosionsequence);
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
                        if(en>99)
                           en=99;
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
                  powerups.removeRef(power);
                  break;
            }
         }
         if(hp>99)
            hp=99;
         ship[pl]->setHitPoints(hp);
      }

      for(mine=mines[pl]->first();mine;mine=mines[pl]->next())
      {
         if(!mine->explodes())
         {
            unexact.clear();
            unexact=mine->collisions(false);
            hitlist.clear();
            for( it=unexact.begin(); it != unexact.end(); ++it )
            {
               sprite = (*it);
               if(sprite->rtti()==S_BULLET)
                  if(mine->collidesWith(sprite))
                     if(!hitlist.contains(sprite))
                        hitlist.append(sprite);
            }
            if(hitlist.count()>0)
            {
               mine->explode(mineexplosionsequence);
               for(it=hitlist.begin(); it != hitlist.end(); ++it)
               {
                  bullet=(BulletSprite*)(*it);
                  bullets[bullet->getPlayerNumber()]->removeRef(bullet);
               }
            }
         }
      }
   }

   hitlist.clear();
   unexact.clear();
   unexact=sun->collisions(false);
   for( it = unexact.begin(); it != unexact.end(); ++it)
   {
      sprite=(*it);
      switch(sprite->rtti())
      {
         case S_BULLET:
            if(sun->collidesWith(sprite))
               if(!hitlist.contains(sprite))
                  hitlist.append(sprite);
            break;
         case S_MINE:
            if(!((MobileSprite*)sprite)->isStopped())
               if(sun->collidesWith(sprite))
                  if(!hitlist.contains(sprite))
                     hitlist.append(sprite);
            break;
      }
   }

   for(it=hitlist.begin(); it != hitlist.end(); ++it)
   {
      sprite=(*it);
      switch(sprite->rtti())
      {
         case S_BULLET:
            bullet=(BulletSprite *)sprite;
            bullet->hide();
            bullets[bullet->getPlayerNumber()]->removeRef(bullet);
            break;
         case S_MINE:
            mine=(MineSprite*)sprite;
            mine->stop();
            if(!mine->explodes())
               mine->explode(mineexplosionsequence);
            break;
      }
   }


   for(pl=0;pl<2;pl++)
   {
      hp=ship[pl]->getHitPoints();
      if(hp!=oldhp[pl])
         emit(hitPoints(pl,hp));
      if((hp==0)&&(ship[pl]->getExplosion()<0))
      {
         op=(pl+1)%2;
         ship[pl]->setExplosion((int)(EXPLOSION_TIME/config.gamespeed));
         expl=new ExplosionSprite(explosionsequence,&field,ship[pl]);
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
   
  if (KConfigDialog::showDialog("settings"))
    return;

  SettingsDialog *settings=new SettingsDialog(&customConfig,this,"settings");
  connect(settings, SIGNAL(settingsUpdated()),this,SLOT(closeSettings()));
  settings->show();
}

void MyMainView::closeSettings(){
  if(Options::lastConfig()<predefinedConfigNum)
    config=modifyConfig(predefinedConfig[Options::lastConfig()]);
  else
    config=modifyConfig(customConfig);
}

Q3CanvasPixmapArray* MyMainView::loadOldPixmapSequence(const QString& datapattern,
                            const QString& maskpattern, int framecount)
{
   int image;
   Q3PtrList<QPixmap> pixmaplist;
   Q3PtrList<QPoint> pointlist;
   QString dataname, maskname;
   QPixmap *pix;
   QBitmap *bitmap;
   int hotx=0, hoty=0;
   QPoint *point;

   for( image=0; image < framecount; image++ )
   {
      // #### Why is this a QString??
      dataname.sprintf( datapattern.toLatin1().constData(), image );
      maskname.sprintf( maskpattern.toLatin1().constData(), image );

      QFile file(dataname);
      if( file.open( QIODevice::ReadOnly ) )
      {
         char line[128];
         file.readLine( line, 128 ); // Skip "P6"/"P3" line
         file.readLine( line, 128 );

         while ( line[0] == '#' )
         {
            // Comment line - see if it has additional parameters
            if ( 0 == strncmp( line,"# HOTSPOT ", 10 ) )
               sscanf( line+10, "%d %d", &hotx, &hoty);

            file.readLine( line, 128 );
         }
         point = new QPoint( hotx, hoty );
         pointlist.append( point );
      }

      pix = new QPixmap( dataname );
      bitmap = new QBitmap( maskname );
      pix->setMask( *bitmap );

      pixmaplist.append( pix );
   }

   Q3CanvasPixmapArray* newarray = new Q3CanvasPixmapArray( pixmaplist, pointlist );
   return newarray;
}

#include "mainview.moc"
