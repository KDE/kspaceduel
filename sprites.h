#ifndef __SPRITE_OBJECTS_H
#define __SPRITE_OBJECTS_H

#include <qcanvas.h>
#include "defines.h"

#ifdef sun
#undef sun
#endif

struct AiSprite
{
   double x,y,dx,dy;
   bool sun, border;
};

class SunSprite:public QCanvasSprite
{
public:
   SunSprite(QCanvasPixmapArray* seq, QCanvas* canvas);
   virtual int rtti() const {return S_SUN;}
};


class PowerupSprite:public QCanvasSprite
{
public:
   enum {PowerupMine=0, PowerupBullet, PowerupShield, PowerupEnergy,
         PowerupNum};
   PowerupSprite(QCanvasPixmapArray* seq, QCanvas* canvas, int t, double lifetime);
   virtual int rtti() const {return S_POWERUP;}

   double getLifetime() {return time;}
   void setLifetime(double t) {time=t;}
   int getType() {return type;}
private:
   double time;
   int type;
};

class MobileSprite:public QCanvasSprite
{
public:
   MobileSprite(QCanvasPixmapArray* array, QCanvas* canvas, int pn);

   virtual void forward(double mult,int frame);
   virtual void forward(double mult);
   virtual void calculateGravity(double gravity,double mult);
   int spriteFieldWidth(){return canvas()->width();}
   int spriteFieldHeight(){return canvas()->height();}
   AiSprite toAiSprite();

   bool isStopped() {return stopped;}
   void stop(bool s=true) {stopped=s;}
   int getPlayerNumber() {return playerNumber;}
protected:
   void checkBounds();
   bool stopped;
   int playerNumber;
};


class ShipSprite:public MobileSprite
{
public:
   ShipSprite(QCanvasPixmapArray* seq, QCanvas* canvas, int pn);
   virtual int rtti() const {return S_SHIP;}
   int getHitPoints() {return hitpoints;}
   void setHitPoints(int hp) {hitpoints=(hp<0?0:hp);}
   double getEnergy() {return energy;}
   void setEnergy(double e) {energy=(e<0?0:e);}
   void setWins(int w) {wins=w;}
   int getWins() {return wins;}
   void setExplosion(int f) {explosion=f;}
   int getExplosion() {return explosion;}
   void setRotation(double r);
   double getRotation() {return rotation;}
   void rotateRight(double rotationEnergyNeed,double rotationSpeed);
   void rotateLeft(double rotationEnergyNeed,double rotationSpeed);
   void bullet(double reloadTime) {reloadBulletTime=reloadTime;}
   bool reloadsBullet(double t=0.0) {return reloadBulletTime>t;}
   void mine(double reloadTime) {reloadMineTime=reloadTime;}
   bool reloadsMine(double t=0.0) {return reloadMineTime>t;}
   bool explodes() {return explosion>=0;}
   void setMinePowerups(int m) {minePowerups=m;}
   int getMinePowerups() {return minePowerups;}
   void setBulletPowerups(int b) {bulletPowerups=b;}
   int getBulletPowerups() {return bulletPowerups;}
   virtual void forward(double mult);
   virtual void forward(double mult,int fr);
   virtual void calculateGravityAndEnergy(double gravity,double sunEnergy,
                                          double mult);
private:
   int hitpoints, wins, explosion;
   double energy,rotation,reloadBulletTime,reloadMineTime;
   int bulletPowerups,minePowerups;
};

class BulletSprite:public MobileSprite
{
public:
   BulletSprite(QCanvasPixmapArray* seq, QCanvas* canvas, int pn,double lifetime);
   virtual int rtti() const {return S_BULLET;}
   virtual void forward(double mult);
   virtual void forward(double mult,int fr);
   bool timeOut() {return time<=0;}
private:
   double time;
};

class MineSprite:public MobileSprite
{
public:
   MineSprite(QCanvasPixmapArray* seq, QCanvas* canvas, int pn,double atime,double f);
   virtual int rtti() const {return S_MINE;}
   bool isActive() {return active;}
   double getFuel() {return fuel;}
   void setFuel(double f) {fuel=(f<0.0?0.0:f);}
   virtual void forward(double mult);
   void explode(QCanvasPixmapArray* s);
   bool explodes() {return expl;}
   bool over() {return (expl&&(explosiontime>(timeToGo-0.1)));}
   virtual void calculateGravity(double gravity,double mult);
private:
   bool expl,active;
   double activateTime,fuel,timeToGo,explosiontime;
};

class ExplosionSprite:public QCanvasSprite
{
public:
   ExplosionSprite(QCanvasPixmapArray *seq, QCanvas* field, MobileSprite *sp);
   virtual int rtti() const {return S_EXPLOSION;}
   bool isOver() {return over;}
   virtual void forward(double mult);
   void setSequence(QCanvasPixmapArray *seq);
private:
   double timeToGo,time;
   bool over;
   MobileSprite *obj;
};

#endif
