#include "mainview.h"
#include <qpixmap.h>
#include <qimage.h>
#include <qpoint.h>
#include <math.h>
#include <qcolor.h>
#include <qkeycode.h>

#include <kapp.h>
#include <kaccel.h>
#include <klocale.h>

#include <stdlib.h>
#include <time.h>
#include "defines.h"


MyMainView::MyMainView(QWidget *parent, const char *name)
      :QWidget(parent,name),
       field((const char *)(MV_PREFIX+MV_BACKGROUND),
             DEF_WIDTH,DEF_HEIGHT,10,200),
       view(&field,this)
{
   int i,p;

   for(p=0;p<2;p++)
   {
      for(i=0;i<PlayerKeyNum;i++)
         playerKeyPressed[p][i]=false;
      bulletShot[p]=false;
      minePut[p]=false;
   }
   for(i=0;i<FunctionKeyNum;i++)
      functionKeyPressed[i]=false;


   QwSpritePixmapSequence *sunsequence
      =new QwSpritePixmapSequence(MV_PREFIX+MV_SUN_PPM,
                                  MV_PREFIX+MV_SUN_PBM);
   sun=new SunSprite(sunsequence);
   sun->moveTo(width()/2-1,height()/2-1);
   sun->show();

   explosionsequence=new QwSpritePixmapSequence(MV_PREFIX+MV_EXPLOSION_PPM,
                                                MV_PREFIX+MV_EXPLOSION_PBM,31);
   mineexplosionsequence=
      new QwSpritePixmapSequence(MV_PREFIX+MV_MINEEX_PPM,
                                 MV_PREFIX+MV_MINEEX_PBM,18);

   shipsequence[0]=new QwSpritePixmapSequence(MV_PREFIX+MV_SHIP1_PPM,
                                              MV_PREFIX+MV_SHIP1_PBM,
                                              ROTNUM);
   shipsequence[1]=new QwSpritePixmapSequence(MV_PREFIX+MV_SHIP2_PPM,
                                              MV_PREFIX+MV_SHIP2_PBM,
                                              ROTNUM);
   ship[0]=new ShipSprite(shipsequence[0],0);
   ship[1]=new ShipSprite(shipsequence[1],1);

   bulletsequence[0]=new QwSpritePixmapSequence(MV_PREFIX+MV_BULLET1_PPM,
                                                MV_PREFIX+MV_BULLET1_PBM);
   bulletsequence[1]=new QwSpritePixmapSequence(MV_PREFIX+MV_BULLET2_PPM,
                                                MV_PREFIX+MV_BULLET2_PBM);
   minesequence[0]=new QwSpritePixmapSequence(MV_PREFIX+MV_MINE1_PPM,
                                              MV_PREFIX+MV_MINE1_PBM,2);
   minesequence[1]=new QwSpritePixmapSequence(MV_PREFIX+MV_MINE2_PPM,
                                              MV_PREFIX+MV_MINE2_PBM,2);
   poverupsequence[PoverupSprite::PoverupMine]
      =new QwSpritePixmapSequence(MV_PREFIX+MV_POVERMINE_PPM,
                                  MV_PREFIX+MV_POVERMINE_PBM);
   poverupsequence[PoverupSprite::PoverupBullet]
      =new QwSpritePixmapSequence(MV_PREFIX+MV_POVERBULLET_PPM,
                                  MV_PREFIX+MV_POVERBULLET_PBM);
   poverupsequence[PoverupSprite::PoverupShield]
      =new QwSpritePixmapSequence(MV_PREFIX+MV_POVERSHIELD_PPM,
                                  MV_PREFIX+MV_POVERSHIELD_PBM);
   poverupsequence[PoverupSprite::PoverupEnergy]
      =new QwSpritePixmapSequence(MV_PREFIX+MV_POVERENERGY_PPM,
                                  MV_PREFIX+MV_POVERENERGY_PBM);
   
   for(i=0;i<2;i++)
   {
      ship[i]->setBoundsAction(QwRealMobileSprite::Wrap);
      ship[i]->hide();
      bullets[i]=new QList<BulletSprite>;
      bullets[i]->setAutoDelete(true);
      mines[i]=new QList<MineSprite>;
      mines[i]->setAutoDelete(true);

   }
      
   explosions.setAutoDelete(true);
   poverups.setAutoDelete(true);

   waitForStart=false;
   textSprite=0;
}

MyMainView::~MyMainView()
{
   killTimers();
}

void MyMainView::readConfig(KConfig *cfg)
{
   int i;
   
   cfg->setGroup("Game");
   options.refreshTime=cfg->readUnsignedNumEntry("refreshTime",33);
   options.lastConfig=cfg->readUnsignedNumEntry("lastConfig",0);
   
   customConfig.gamespeed=cfg->readDoubleNumEntry("gamespeed",
                                                  predefinedConfig[0].gamespeed);

   options.isAi[0]=cfg->readBoolEntry("player1IsAi",false);
   options.isAi[1]=cfg->readBoolEntry("player2IsAi",false);
   options.aiDifficulty[0]=
      (Difficulty)cfg->readUnsignedNumEntry("ai1Difficulty",(unsigned)DNORMAL);
   options.aiDifficulty[1]=
      (Difficulty)cfg->readUnsignedNumEntry("ai2Difficulty",(unsigned)DNORMAL);

   options.timeAfterKill=
      cfg->readDoubleNumEntry("timeAfterKill",1.3*33);
   customConfig.gravity=
      cfg->readDoubleNumEntry("gravity",predefinedConfig[0].gravity);
   customConfig.acc=
      cfg->readDoubleNumEntry("acceleration",predefinedConfig[0].acc);
   customConfig.bulletDamage=
      cfg->readUnsignedNumEntry("bulletDamage",predefinedConfig[0].bulletDamage);
   customConfig.bulletLifeTime=
      cfg->readDoubleNumEntry("bulletLifeTime",predefinedConfig[0].bulletLifeTime);
   customConfig.bulletReloadTime=
      cfg->readDoubleNumEntry("bulletReloadTime",predefinedConfig[0].bulletReloadTime);
   customConfig.mineDamage=
      cfg->readUnsignedNumEntry("mineDamage",predefinedConfig[0].mineDamage);
   customConfig.shipDamage=
      cfg->readUnsignedNumEntry("shipDamage",predefinedConfig[0].shipDamage);
   customConfig.maxBullets=
      cfg->readUnsignedNumEntry("maxBullets",predefinedConfig[0].maxBullets);
   customConfig.maxMines=
      cfg->readUnsignedNumEntry("maxMines",predefinedConfig[0].maxMines);
   customConfig.mineReloadTime=
      cfg->readDoubleNumEntry("mineReloadTime",predefinedConfig[0].mineReloadTime);
   customConfig.rotationSpeed=
      cfg->readDoubleNumEntry("rotationSpeed",predefinedConfig[0].rotationSpeed);
   customConfig.shotSpeed=
      cfg->readDoubleNumEntry("shotSpeed",predefinedConfig[0].shotSpeed);
   customConfig.energyNeed=
      cfg->readDoubleNumEntry("accEnergyNeed",predefinedConfig[0].energyNeed);
   customConfig.rotationEnergyNeed=
      cfg->readDoubleNumEntry("rotationEnergyNeed",predefinedConfig[0].rotationEnergyNeed);
   customConfig.sunEnergy=
      cfg->readDoubleNumEntry("sunEnergy",predefinedConfig[0].sunEnergy);
   customConfig.mineActivateTime=
      cfg->readDoubleNumEntry("mineActivateTime",predefinedConfig[0].mineActivateTime);
   customConfig.mineFuel=
      cfg->readDoubleNumEntry("mineFuel",predefinedConfig[0].mineFuel);
   customConfig.shotEnergyNeed=
      cfg->readDoubleNumEntry("shotEnergyNeed",predefinedConfig[0].shotEnergyNeed);
   customConfig.mineEnergyNeed=
      cfg->readDoubleNumEntry("mineEnergyNeed",predefinedConfig[0].mineEnergyNeed);
   customConfig.startPosX=
      cfg->readDoubleNumEntry("startPosX",predefinedConfig[0].startPosX);
   customConfig.startPosY=
      cfg->readDoubleNumEntry("startPosY",predefinedConfig[0].startPosY);
   customConfig.startVelX=
      cfg->readDoubleNumEntry("startVelX",predefinedConfig[0].startVelX);
   customConfig.startVelY=
      cfg->readDoubleNumEntry("startVelY",predefinedConfig[0].startVelY);
   customConfig.poverupLifeTime=
      cfg->readDoubleNumEntry("poverupLifeTime",predefinedConfig[0].poverupLifeTime);
   customConfig.poverupRefreshTime=
      cfg->readDoubleNumEntry("poverupRefreshTime",predefinedConfig[0].poverupRefreshTime);
   customConfig.poverupShieldAmount=
      cfg->readUnsignedNumEntry("poverupShieldAmount",
                              predefinedConfig[0].poverupShieldAmount);
   customConfig.poverupEnergyAmount=
      cfg->readDoubleNumEntry("poverupEnergyAmount",
                              predefinedConfig[0].poverupEnergyAmount);

   options.functionKey[FunctionKeyStart]=stringToKey(cfg->readEntry("KeyStart","Space"));

   cfg->setGroup("Player2");
   options.playerKey[1][PlayerKeyLeft]=stringToKey(cfg->readEntry("KeyLeft","Left"));
   options.playerKey[1][PlayerKeyRight]=stringToKey(cfg->readEntry("KeyRight","Right"));
   options.playerKey[1][PlayerKeyAcc]=stringToKey(cfg->readEntry("KeyAcc","Up"));
   options.playerKey[1][PlayerKeyShot]=stringToKey(cfg->readEntry("KeyShot","Down"));
   options.playerKey[1][PlayerKeyMine]=stringToKey(cfg->readEntry("KeyMine","Control"));
   options.startHitPoints[1]=cfg->readUnsignedNumEntry("startHitPoints",99);
   cfg->setGroup("Player1");
   options.playerKey[0][PlayerKeyLeft]=stringToKey(cfg->readEntry("KeyLeft","S"));
   options.playerKey[0][PlayerKeyRight]=stringToKey(cfg->readEntry("KeyRight","F"));
   options.playerKey[0][PlayerKeyAcc]=stringToKey(cfg->readEntry("KeyAcc","E"));
   options.playerKey[0][PlayerKeyShot]=stringToKey(cfg->readEntry("KeyShot","D"));
   options.playerKey[0][PlayerKeyMine]=stringToKey(cfg->readEntry("KeyMine","A"));
   options.startHitPoints[0]=cfg->readUnsignedNumEntry("startHitPoints",99);
   
   if(options.lastConfig<predefinedConfigNum)
      config=modifyConfig(predefinedConfig[options.lastConfig]);
   else
      config=modifyConfig(customConfig);

   for(i=0;i<2;i++)
      ai[i]=new Ai(i,ship,bullets,mines,&config,&options);
}

void MyMainView::writeConfig()
{
   KConfig *cfg;
   cfg=KApplication::getKApplication()->getConfig();
   cfg->setGroup("Game");
   cfg->writeEntry("refreshTime",options.refreshTime);
   cfg->writeEntry("lastConfig",options.lastConfig);
   cfg->writeEntry("gamespeed",customConfig.gamespeed);
   cfg->writeEntry("timeAfterKill",options.timeAfterKill);
      
   cfg->writeEntry("gravity",customConfig.gravity);
   cfg->writeEntry("acceleration",customConfig.acc);
   cfg->writeEntry("bulletDamage",customConfig.bulletDamage);
   cfg->writeEntry("bulletLifeTime",customConfig.bulletLifeTime);
   cfg->writeEntry("bulletReloadTime",customConfig.bulletReloadTime);
   cfg->writeEntry("mineDamage",customConfig.mineDamage);
   cfg->writeEntry("shipDamage",customConfig.shipDamage);
   cfg->writeEntry("maxBullets",customConfig.maxBullets);
   cfg->writeEntry("maxMines",customConfig.maxMines);
   cfg->writeEntry("rotationSpeed",customConfig.rotationSpeed);
   cfg->writeEntry("shotSpeed",customConfig.shotSpeed);
   cfg->writeEntry("accEnergyNeed",customConfig.energyNeed);
   cfg->writeEntry("rotationEnergyNeed",customConfig.rotationEnergyNeed);
   cfg->writeEntry("sunEnergy",customConfig.sunEnergy);
   cfg->writeEntry("mineActivateTime",customConfig.mineActivateTime);
   cfg->writeEntry("mineReloadTime",customConfig.mineReloadTime);
   cfg->writeEntry("mineFuel",customConfig.mineFuel);
   cfg->writeEntry("shotEnergyNeed",customConfig.shotEnergyNeed);
   cfg->writeEntry("mineEnergyNeed",customConfig.mineEnergyNeed);
      
   cfg->writeEntry("startPosX",customConfig.startPosX);
   cfg->writeEntry("startPosY",customConfig.startPosY);
   cfg->writeEntry("startVelX",customConfig.startVelX);
   cfg->writeEntry("startVelY",customConfig.startVelY);

   cfg->writeEntry("poverupLifeTime",customConfig.poverupLifeTime);
   cfg->writeEntry("poverupRefreshTime",customConfig.poverupRefreshTime);
   cfg->writeEntry("poverupShieldAmount",customConfig.poverupShieldAmount);
   cfg->writeEntry("poverupEnergyAmount",customConfig.poverupEnergyAmount);
      
   cfg->setGroup("Player2");
   cfg->writeEntry("startHitPoints",options.startHitPoints[1]);
   cfg->setGroup("Player1");
   cfg->writeEntry("startHitPoints",options.startHitPoints[0]);

   cfg->setGroup("Player2");
   cfg->writeEntry("KeyLeft",keyToString(options.playerKey[1][PlayerKeyLeft]));
   cfg->writeEntry("KeyRight",keyToString(options.playerKey[1][PlayerKeyRight]));
   cfg->writeEntry("KeyAcc",keyToString(options.playerKey[1][PlayerKeyAcc]));
   cfg->writeEntry("KeyShot",keyToString(options.playerKey[1][PlayerKeyShot]));
   cfg->writeEntry("KeyMine",keyToString(options.playerKey[1][PlayerKeyMine]));
   cfg->setGroup("Player1");
   cfg->writeEntry("KeyLeft",keyToString(options.playerKey[0][PlayerKeyLeft]));
   cfg->writeEntry("KeyRight",keyToString(options.playerKey[0][PlayerKeyRight]));
   cfg->writeEntry("KeyAcc",keyToString(options.playerKey[0][PlayerKeyAcc]));
   cfg->writeEntry("KeyShot",keyToString(options.playerKey[0][PlayerKeyShot]));
   cfg->writeEntry("KeyMine",keyToString(options.playerKey[0][PlayerKeyMine]));
   cfg->setGroup("Game");
   cfg->writeEntry("KeyStart",keyToString(options.functionKey[FunctionKeyStart]));
   cfg->writeEntry("player1IsAi",options.isAi[0]);
   cfg->writeEntry("player2IsAi",options.isAi[1]);
   cfg->writeEntry("ai1Difficulty",(unsigned)options.aiDifficulty[0]);
   cfg->writeEntry("ai2Difficulty",(unsigned)options.aiDifficulty[1]);
}

SConfig MyMainView::modifyConfig(SConfig conf)
{
   SConfig newConfig=conf;
   newConfig.gamespeed*=options.refreshTime/33.0;
   newConfig.acc*=newConfig.gamespeed;
   newConfig.rotationSpeed*=newConfig.gamespeed*M_PI/ROTNUM*4;
   newConfig.energyNeed*=newConfig.gamespeed;
   newConfig.rotationEnergyNeed*=newConfig.gamespeed;
   newConfig.mineActivateTime*=newConfig.gamespeed;

   return newConfig;
}

void MyMainView::keyPressEvent(QKeyEvent *ev)
{
   int key=ev->key();
   int i,p;
   bool accept=false;

   if((gameEnd<=0.0)&&(gameEnd>-2.0))
   {
      if(key==options.functionKey[FunctionKeyStart])
         newRound();
   }
   else if(waitForStart)
   {
      if((key==options.functionKey[FunctionKeyStart])
         && (!functionKeyPressed[FunctionKeyStart]))
      {
         functionKeyPressed[FunctionKeyStart]=true;
         resume();
      }
   }
   else
   {
      if((key==options.functionKey[FunctionKeyStart])
         && (!functionKeyPressed[FunctionKeyStart]))
      {
         functionKeyPressed[FunctionKeyStart]=true;
         pause();
      }
      for(p=0;p<2;p++)
         for(i=0;i<PlayerKeyNum;i++)
            if(key==options.playerKey[p][i])
            {
               playerKeyPressed[p][i]=true;
               accept=true;
            }
      if(!accept)
         ev->ignore();
   }
   
}

void MyMainView::keyReleaseEvent(QKeyEvent *ev)
{
   int i,p,key=ev->key();
   bool accept=false;

   for(i=0;i<FunctionKeyNum;i++)
      if(key==options.functionKey[i])
      {
         functionKeyPressed[i]=false;
         accept=true;
      }
   if(!accept)
      for(p=0;p<2;p++)
         for(i=0;i<PlayerKeyNum;i++)
            if(key==options.playerKey[p][i])
            {
               playerKeyPressed[p][i]=false;
               accept=true;
            }
   if(!accept)
      ev->ignore();
}

void MyMainView::pause()
{
   waitForStart=true;
   killTimers();
   QString str;
   str.sprintf(i18n("Press %s to start/resume"),
              (const char*)keyToString(options.functionKey[FunctionKeyStart]));
   emit(setStatusText(i18n(" paused "),IDS_PAUSE));  
   emit(setStatusText(str,IDS_MAIN));
}

void MyMainView::resume()
{
   waitForStart=false;
   timerID=startTimer(options.refreshTime);
   emit(setStatusText("",IDS_PAUSE));
   emit(setStatusText("",IDS_MAIN));
}

void MyMainView::resizeEvent(QResizeEvent *event)
{
   double mx,my;
   MineSprite *mine;
   BulletSprite *bullet;
   PoverupSprite *poverup;
   int i,current;

   mx=(event->size().width()-event->oldSize().width())/2.0;
   my=(event->size().height()-event->oldSize().height())/2.0;
   QWidget::resizeEvent(event);
   view.resize(width(),height());
   field.resize(width(),height());
   sun->moveTo(width()/2-1,height()/2-1);

   for(i=0;i<2;i++)
   {
      ship[i]->adoptSpritefieldBounds();
      ship[i]->moveBy(mx,my);
      current=mines[i]->at();
      for(mine=mines[i]->first();mine;mine=mines[i]->next())
      {
         mine->adoptSpritefieldBounds();
         mine->moveBy(mx,my);
      }
      if(current>=0)
         mines[i]->at(current);

      current=bullets[i]->at();
      for(bullet=bullets[i]->first();bullet;bullet=bullets[i]->next())
      {
         bullet->adoptSpritefieldBounds();
         bullet->moveBy(mx,my);
      }
      if(current>=0)
         bullets[i]->at(current);

   }
   if(textSprite)
      textSprite->moveBy((int)mx,(int)my);
   current=poverups.at();
   for(poverup=poverups.first();poverup;poverup=poverups.next())
      poverup->moveBy(mx,my);
   if(current>=0)
      poverups.at(current);
}

void MyMainView::newRound()
{
   double mx,my;
   int i;

   srand(time(NULL));
   
   timeToNextPoverup=(double)(rand() % (int)config.poverupRefreshTime);
   poverups.clear();

   killTimers();
   mx=width()/2.0;
   my=height()/2.0;
   ship[0]->moveTo(mx+config.startPosX,my+config.startPosY);
   ship[0]->setRotation(0.0);
   ship[0]->frame(0);
   
   ship[1]->moveTo(mx-config.startPosX,my-config.startPosY);
   ship[1]->setRotation(M_PI);
   ship[1]->frame(ROTNUM/2);

   ship[0]->setVelocity(config.startVelX,config.startVelY);
   ship[1]->setVelocity(-config.startVelX,-config.startVelY);
   for(i=0;i<2;i++)
   {
      ship[i]->show();
      ship[i]->setEnergy(99.9);
      ship[i]->setHitPoints(options.startHitPoints[i]);
      ship[i]->stop(false);
      ship[i]->setExplosion(-1);
      emit(energy(i,(int)ship[i]->getEnergy()));
      emit(hitPoints(i,ship[i]->getHitPoints()));
      bulletShot[i]=false;
      bullets[i]->clear();
      mines[i]->clear();
      ship[i]->mine(0.0);
      ship[i]->bullet(0.0);
      ship[i]->setBulletPoverups(0);
      ship[i]->setMinePoverups(0);

      ai[i]->newRound();
   }
   explosions.clear();
   pause();
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
   for(i=0;i<FunctionKeyNum;i++)
      functionKeyPressed[i]=false;
}

void MyMainView::timerEvent(QTimerEvent *event)
{
   unsigned w;
   int i;

   if(event->timerId()==timerID)
   {
      killTimers();
      if(gameEnd>0.0)
      {
         gameEnd-=1.0;
         if(gameEnd<=0.0)
         {
            textSprite=new QwTextSprite;
            textSprite->moveTo(width()/2,height()/2-90);
            textSprite->setTextFlags(AlignCenter);
            textSprite->setColor(qRgb(255,160,0));
            textSprite->setFont(QFont("Helvetica",14));
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
            QString str;
            str.sprintf(i18n("Press %s for new round"),
                        (const char*)keyToString(
                           options.functionKey[FunctionKeyStart]));
            emit(setStatusText(str,IDS_MAIN));
         }
         else
            timerID=startTimer(options.refreshTime);      
      }
      else
         timerID=startTimer(options.refreshTime);

      for(i=0;i<2;i++)
         if(options.isAi[i]&&(ship[i]->getHitPoints()>0))
            ai[i]->think();
      
      moveMines();
      moveBullets();
      moveExplosions();
      moveShips();
      calculatePoverups();
      collisions();
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

         
         if(!options.isAi[i]&&playerKeyPressed[i][PlayerKeyRight]
            || options.isAi[i]&&ai[i]->rotateRight())
            ship[i]->rotateRight(config.rotationEnergyNeed,
                                 config.rotationSpeed);

         if(!options.isAi[i]&&playerKeyPressed[i][PlayerKeyLeft]
            || options.isAi[i]&&ai[i]->rotateLeft())
            ship[i]->rotateLeft(config.rotationEnergyNeed,
                                config.rotationSpeed);
 
         en=ship[i]->getEnergy();
         nr=ship[i]->getRotation();

         
         nf=ship[i]->frame();
         nx=cos(nr);
         ny=sin(nr);
         if((!options.isAi[i]&&playerKeyPressed[i][PlayerKeyAcc]
             || options.isAi[i]&&ai[i]->accelerate())
            &&(en>config.energyNeed))
         {
            en-=config.energyNeed;
            ship[i]->setVelocity(ship[i]->dX()+nx*config.acc,
                                 ship[i]->dY()-ny*config.acc);
         }
         if(en>99.9)
            en=99.9;

         ship[i]->forward(config.gamespeed);
             //Bullets and Mines
         if(!options.isAi[i]&&playerKeyPressed[i][PlayerKeyShot]
            ||options.isAi[i]&&ai[i]->shootBullet())
         {
            if((en>config.shotEnergyNeed) && (!ship[i]->reloadsBullet()))
            {
               if(bullets[i]->count() <
                  (config.maxBullets+ship[i]->getBulletPoverups()))
               {
                  ship[i]->bullet(config.bulletReloadTime);
                  en-=config.shotEnergyNeed;
                  bullet=new BulletSprite(bulletsequence[i],i,
                                          config.bulletLifeTime);
                  bullet->moveTo(ship[i]->exact_x()+nx*SHOTDIST,
                                 ship[i]->exact_y()-ny*SHOTDIST);
                  bullet->setVelocity(ship[i]->dX()+nx*config.shotSpeed,
                                      ship[i]->dY()-ny*config.shotSpeed);
                  bullet->setBoundsAction(QwRealMobileSprite::Wrap);
                  bullet->show();
                  
                  bullets[i]->append(bullet);
               }
            }
         }
         if(!options.isAi[i]&&playerKeyPressed[i][PlayerKeyMine]
            || options.isAi[i]&&ai[i]->layMine())
         {
            if((en>config.mineEnergyNeed) && (!ship[i]->reloadsMine()))
            {
               if(mines[i]->count() <
                  (config.maxMines+ship[i]->getMinePoverups()))
               {
                  ship[i]->mine(config.mineReloadTime);
                  en-=config.mineEnergyNeed;
                  mine=new MineSprite(minesequence[i],i,
                                      config.mineActivateTime,config.mineFuel);
                  mine->moveTo(ship[i]->exact_x(),ship[i]->exact_y());
                  mine->setVelocity(0,0);
                  mine->setBoundsAction(QwRealMobileSprite::Wrap);
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

void MyMainView::calculatePoverups()
{
   PoverupSprite *sp;
   int type,x,y;

   sp=poverups.first();
   while(sp)
   {
      sp->setLifetime(sp->getLifetime()-config.gamespeed);
      if(sp->getLifetime()<0)
      {
         poverups.removeRef(sp);
         sp=poverups.current();
      }
      else
         sp=poverups.next();
   }
   timeToNextPoverup-=config.gamespeed;
   if(timeToNextPoverup<0)
   {
      timeToNextPoverup=(double)(rand() % (int)config.poverupRefreshTime);
      type=rand() % PoverupSprite::PoverupNum;
      sp=new PoverupSprite(poverupsequence[type],type,config.poverupLifeTime);
      do
      {
         x=(rand()%(width()-40))+20;
         y=(rand()%(height()-40))+20;
      }
      while(((x-width()/2)*(x-width()/2)+(y-height()/2)*(y-height()/2))<(50*50));
      sp->moveTo(x,y);
      poverups.append(sp);
      sp->show();
   }
}

void MyMainView::collisions()
{
   int pl,hp,op,oldhp[2],ohp;
   Pix pix;
   QwSpriteFieldGraphic *sprite;
   BulletSprite *bullet;
   MineSprite *mine;
   ExplosionSprite *expl;
   ShipSprite *s;
   PoverupSprite *pover;
   QList<QwSpriteFieldGraphic> hitlist;
   double ndx[2],ndy[2];
   double en;

   for(pl=0;pl<2;pl++)
   {
      if(!ship[pl]->isStopped())
      {
         pix=ship[pl]->neighbourhood(ship[pl]->frame());
         oldhp[pl]=hp=ship[pl]->getHitPoints();
         hitlist.clear();
         while(pix)
         {
            sprite=ship[pl]->at(pix);
            if(sprite)
               if((sprite->rtti()!=S_EXPLOSION)
                  && !((sprite->rtti()!=S_SUN)&&(ship[pl]->getHitPoints()==0)))
                  if(ship[pl]->exact(pix))
                     if(!hitlist.containsRef(sprite))
                        hitlist.append(sprite);
            ship[pl]->next(pix);
         }
         ship[pl]->end(pix);

         for(sprite=hitlist.first();sprite;sprite=hitlist.next())
         {
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
                     ndx[0]=((1-EPSILON)*ship[0]->dX()+(1+EPSILON)*ship[1]->dX())/2.0;
                     ndy[0]=((1-EPSILON)*ship[0]->dY()+(1+EPSILON)*ship[1]->dY())/2.0;
                     ndx[1]=((1-EPSILON)*ship[1]->dX()+(1+EPSILON)*ship[0]->dX())/2.0;
                     ndy[1]=((1-EPSILON)*ship[1]->dY()+(1+EPSILON)*ship[0]->dY())/2.0;
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
                     ndx[0]=(ship[pl]->dX()+0.3*mine->dX())/1.3;
                     ndy[0]=(ship[pl]->dY()+0.3*mine->dY())/1.3;
                     ship[pl]->setVelocity(ndx[0],ndy[0]);
                     mine->setVelocity(ndx[0],ndy[0]);
                     hp-=config.mineDamage;
                  }
                  break;
               case S_POVERUP:
                  pover=(PoverupSprite *)sprite;
                  switch(pover->getType())
                  {
                     case PoverupSprite::PoverupShield:
                        hp+=config.poverupShieldAmount;
                        break;
                     case PoverupSprite::PoverupEnergy:
                        en=ship[pl]->getEnergy()+config.poverupEnergyAmount;
                        if(en>99)
                           en=99;
                        ship[pl]->setEnergy(en);
                        break;
                     case PoverupSprite::PoverupMine:
                        ship[pl]->setMinePoverups(
                           ship[pl]->getMinePoverups()+1);
                        break;
                     case PoverupSprite::PoverupBullet:
                        ship[pl]->setBulletPoverups(
                           ship[pl]->getMinePoverups()+1);
                        break;
                  }
                  pover->hide();
                  poverups.removeRef(pover);
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
            pix=mine->neighbourhood(mine->frame());
            hitlist.clear();
            while(pix)
            {
               sprite=mine->at(pix);
               if(sprite)
                  if(sprite->rtti()==S_BULLET)
                     if(mine->exact(pix))
                        if(!hitlist.containsRef(sprite))
                           hitlist.append(sprite);
               mine->next(pix);
            }
            mine->end(pix);
            if(hitlist.count()>0)
            {
               mine->explode(mineexplosionsequence);
               for(sprite=hitlist.first();sprite;sprite=hitlist.next())
               {
                  bullet=(BulletSprite*)sprite;
                  bullets[bullet->getPlayerNumber()]->removeRef(bullet);
               }
            }
         }
      }
   }
   pix=sun->neighbourhood(sun->frame());
   hitlist.clear();
   while(pix)
   {
      sprite=sun->at(pix);
      switch(sprite->rtti())
      {
         case S_BULLET:
            if(sun->exact(pix))
               if(!hitlist.containsRef(sprite))
                  hitlist.append(sprite);
            break;
         case S_MINE:
            if(!((MobileSprite*)sprite)->isStopped())
               if(sun->exact(pix))
                  if(!hitlist.containsRef(sprite))
                     hitlist.append(sprite);
            break;
      }
      sun->next(pix);
   }
   sun->end(pix);
   for(sprite=hitlist.first();sprite;sprite=hitlist.next())
   {
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
         expl=new ExplosionSprite(explosionsequence,ship[pl]);
         expl->show();
         explosions.append(expl);
         gameEnd=options.timeAfterKill/config.gamespeed;
      }
   }
}

void MyMainView::gameSetup()
{
   if(!waitForStart)
      pause();
   ConfigSetup dialog(&customConfig,&options,this);
   dialog.exec();
   if(options.lastConfig<predefinedConfigNum)
      config=modifyConfig(predefinedConfig[options.lastConfig]);
   else
      config=modifyConfig(customConfig);
}

void MyMainView::keySetup()
{
   if(!waitForStart)
      pause();
   KeySetup dialog(&options,this);
   dialog.exec();
   QString str;
   str.sprintf(i18n("Press %s to start/resume"),
              (const char*)keyToString(options.functionKey[FunctionKeyStart]));
   emit(setStatusText(str,IDS_MAIN));
}

void MyMainView::aiSetup()
{
   if(!waitForStart)
      pause();
   AiSetup dialog(&options,this);
   dialog.exec();
}

void MyMainView::hitpointSetup()
{
   if(!waitForStart)
      pause();
   HitpointSetup dialog(&options,this);
   dialog.exec();
}

void MyMainView::graphicSetup()
{
   if(!waitForStart)
      pause();
   GraphicSetup dialog(&options,this);
   dialog.exec();
}
