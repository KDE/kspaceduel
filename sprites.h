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

#ifndef __SPRITE_OBJECTS_H
#define __SPRITE_OBJECTS_H

#include <QGraphicsPixmapItem>
class QGraphicsScene;

#include "defines.h"

#ifdef sun
#undef sun
#endif

struct AiSprite
{
   double x,y,dx,dy;
   bool sun, border;
};

class SimpleSprite: public QGraphicsPixmapItem
{
   public:
      int width();
      int height();
      QPointF center();
   private:
      void init();
      
      int m_width;
      int m_height;
      QPointF m_center;
   protected:
      SimpleSprite(QPixmap* pixmap, QGraphicsScene* scene);
      SimpleSprite(QGraphicsItem* parent = 0, QGraphicsScene * scene = 0);
};

class SunSprite:public SimpleSprite
{
public:
   SunSprite(QPixmap* pixmap, QGraphicsScene* scene);
   virtual int type() const {return S_SUN;}
};

class PowerupSprite:public SimpleSprite
{
public:
   enum {PowerupMine=0, PowerupBullet, PowerupShield, PowerupEnergy,
         PowerupNum};
   PowerupSprite(QPixmap* pixmap, QGraphicsScene* scene, int t, double lifetime);
   virtual int type() const {return S_POWERUP;}

   double getLifetime() {return time;}
   void setLifetime(double t) {time=t;}
   int getType() {return mtype;}
private:
   double time;
   int mtype;
};

class MobileSprite:public SimpleSprite
{
public:
   MobileSprite(QPixmap* pixmap, QGraphicsScene* scene, int pn);

   virtual void forward(double mult,int frame);
   virtual void forward(double mult);
   virtual void calculateGravity(double gravity,double mult);
   int spriteFieldWidth();
   int spriteFieldHeight();
   double xVelocity(){return xvel;}
   double yVelocity(){return yvel;}
   void setVelocity(double vx, double vy);
   AiSprite toAiSprite();

   bool isStopped() {return stopped;}
   void stop(bool s=true) {stopped=s;}
   int getPlayerNumber() {return playerNumber;}
protected:
   MobileSprite(QGraphicsItem* parent = 0, QGraphicsScene * scene = 0, int pn = 0);
	
   void checkBounds();
   bool stopped;
   int playerNumber;
   double xvel;
   double yvel;
};

class AnimatedSprite:public MobileSprite
{
public:
   explicit AnimatedSprite(const QList<QPixmap> &animation, QGraphicsScene *scene = 0, int pn=0);

   void setFrame(int frame);
   inline int frame() const
   { return currentFrame; }
   inline int frameCount() const
   { return frames.size(); }
   inline QPixmap image(int frame) const
   { return frames.isEmpty() ? QPixmap() : frames.at(frame % frames.size()); }
   QPainterPath shape() const;
   void setAnimation(const QList<QPixmap> &animation);

   void advance(int phase);

private:
   int currentFrame;
   QList<QPixmap> frames;
};

class ShipSprite:public MobileSprite
{
public:
   ShipSprite(QPixmap* pixmap, QGraphicsScene* scene, int pn);
   virtual int type() const {return S_SHIP;}
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
   double angle; // current rotation angle
};

class BulletSprite:public MobileSprite
{
public:
   BulletSprite(QPixmap* pixmap, QGraphicsScene* scene, int pn,double lifetime);
   virtual int type() const {return S_BULLET;}
   virtual void forward(double mult);
   virtual void forward(double mult,int fr);
   bool timeOut() {return time<=0;}
private:
   double time;
};

class MineSprite:public AnimatedSprite
{
public:
   MineSprite(const QList<QPixmap> &animation, const QList<QPixmap> &exploanimation, QGraphicsScene* scene, int pn,double atime,double f);
   virtual int type() const {return S_MINE;}
   bool isActive() {return active;}
   double getFuel() {return fuel;}
   void setFuel(double f) {fuel=(f<0.0?0.0:f);}
   virtual void forward(double mult);
   void explode();
   bool explodes() {return expl;}
   bool over() {return (expl&&(explosiontime>(timeToGo-0.1)));}
   virtual void calculateGravity(double gravity,double mult);
private:
   bool expl,active;
   double activateTime,fuel,timeToGo,explosiontime;
   QList<QPixmap> exploframes; // when mine explodes, we set frames to exploframes (needed because both player's mines have
   				// the same explosion animation
};

class ExplosionSprite:public AnimatedSprite
{
public:
   explicit ExplosionSprite(const QList<QPixmap> &animation, QGraphicsScene *scene = 0, MobileSprite *sp = 0);
   virtual int type() const {return S_EXPLOSION;}
   bool isOver() {return over;}
   virtual void forward(double mult);
private:
   double timeToGo,time;
   bool over;
   MobileSprite *obj;
};

#endif
