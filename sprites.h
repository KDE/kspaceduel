#ifndef __SPRITE_OBJECTS_H
#define __SPRITE_OBJECTS_H

#include <QwSpriteField.h>
#include "defines.h"

#ifdef sun
#undef sun
#endif

struct AiSprite
{
   double x,y,dx,dy;
   bool sun, border;
};

class SunSprite:public QwSprite
{
public:
   SunSprite(QwSpritePixmapSequence *seq);
   virtual int rtti() const {return S_SUN;};
};

class MobileSprite:public QwRealMobileSprite
{
public:
   MobileSprite(QwSpritePixmapSequence *seq,int pn);
   MobileSprite(int pn);

   virtual void forward(double mult,int frame);
   virtual void forward(double mult);
   virtual void calculateGravity(double gravity,double mult);
   int spriteFieldWidth(){return spritefield->width();}
   int spriteFieldHeight(){return spritefield->height();}
   AiSprite toAiSprite();

   bool isStopped() {return stopped;}
   void stop(bool s=true) {stopped=s;}
   int getPlayerNumber() {return playerNumber;}
protected:
   bool stopped;
   int playerNumber;
};


class ShipSprite:public MobileSprite
{
public:
   ShipSprite(QwSpritePixmapSequence *seq,int pn);
   ShipSprite(int pn);
   virtual int rtti() const {return S_SHIP;};
   int getHitPoints() {return hitpoints;};
   void setHitPoints(int hp) {hitpoints=(hp<0?0:hp);};
   double getEnergy() {return energy;};
   void setEnergy(double e) {energy=(e<0?0:e);};
   void setWins(int w) {wins=w;};
   int getWins() {return wins;};
   void setExplosion(int f) {explosion=f;};
   int getExplosion() {return explosion;};
   void setRotation(double r);
   double getRotation() {return rotation;};
   void rotateRight(double rotationEnergyNeed,double rotationSpeed);
   void rotateLeft(double rotationEnergyNeed,double rotationSpeed);
   void bullet(double reloadTime) {reloadBulletTime=reloadTime;};
   bool reloadsBullet(double t=0.0) {return reloadBulletTime>t;};
   void mine(double reloadTime) {reloadMineTime=reloadTime;};
   bool reloadsMine(double t=0.0) {return reloadMineTime>t;};
   bool explodes() {return explosion>=0;};
   virtual void forward(double mult);
   virtual void forward(double mult,int fr);
   virtual void calculateGravityAndEnergy(double gravity,double sunEnergy,
                                          double mult);
private:
   int hitpoints, wins, explosion;
   double energy,rotation,reloadBulletTime,reloadMineTime;
};

class BulletSprite:public MobileSprite
{
public:
   BulletSprite(QwSpritePixmapSequence *seq,int pn,double lifetime);
   BulletSprite(int pn,double lifetime);
   virtual int rtti() const {return S_BULLET;};
   virtual void forward(double mult);
   virtual void forward(double mult,int fr);
   bool timeOut() {return time<=0;};
private:
   double time;
};

class MineSprite:public MobileSprite
{
public:
   MineSprite(QwSpritePixmapSequence *seq,int pn,double atime,double f);
   MineSprite(int pn,double atime,double f);
   virtual int rtti() const {return S_MINE;};
   bool isActive() {return active;};
   double getFuel() {return fuel;};
   void setFuel(double f) {fuel=(f<0.0?0.0:f);};
   virtual void forward(double mult);
   void explode(QwSpritePixmapSequence* s);
   bool explodes() {return expl;};
   bool over() {return (expl&&(explosiontime>(timeToGo-0.1)));};
   virtual void calculateGravity(double gravity,double mult);
private:
   bool expl,active;
   double activateTime,fuel,timeToGo,explosiontime;
};

class ExplosionSprite:public QwRealSprite
{
public:
   ExplosionSprite(QwSpritePixmapSequence *seq,MobileSprite *sp);
   ExplosionSprite(MobileSprite *sp);
   virtual int rtti() const {return S_EXPLOSION;};
   bool isOver() {return over;};
   virtual void forward(double mult);
   void setSequence(QwSpritePixmapSequence *seq);
private:
   double timeToGo,time;
   bool over;
   MobileSprite *obj;
};

#endif
