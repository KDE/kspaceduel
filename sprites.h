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

#ifndef KSPACEDUEL_SPRITES_H
#define KSPACEDUEL_SPRITES_H

#include "defines.h"
#include "spritebase.h"

#ifdef sun
#undef sun
#endif

class SunSprite:public SimpleSprite
{
public:
   SunSprite(QSvgRenderer* svg, const QString& element);
   int type() const Q_DECL_OVERRIDE {return S_SUN;}
};

class PowerupSprite:public SimpleSprite
{
public:
   enum {PowerupMine=0, PowerupBullet, PowerupShield, PowerupEnergy,
         PowerupNum};
   PowerupSprite(QSvgRenderer* svg, const QString& element, int t, double lifetime);
   int type() const Q_DECL_OVERRIDE {return S_POWERUP;}

   double getLifetime() {return time;}
   void setLifetime(double t) {time=t;}
   int getType() {return mtype;}
private:
   double time;
   int mtype;
};

class ShipSprite:public MobileSprite
{
public:
   ShipSprite(QSvgRenderer* svg, const QString& element, int pn);
   int type() const Q_DECL_OVERRIDE {return S_SHIP;}
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
   void forward(double mult) Q_DECL_OVERRIDE;
   void forward(double mult,int fr) Q_DECL_OVERRIDE;
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
   BulletSprite(QSvgRenderer* svg, const QString& elem, int pn,double lifetime);
   int type() const Q_DECL_OVERRIDE {return S_BULLET;}
   void forward(double mult) Q_DECL_OVERRIDE;
   void forward(double mult,int fr) Q_DECL_OVERRIDE;
   bool timeOut() {return time<=0;}
private:
   double time;
};

class MineSprite:public AnimatedSprite
{
public:
   MineSprite(QSvgRenderer* svg, const QList<QString> &animation, const QList<QString> &exploanimation, int pn,double atime,double f);
   int type() const Q_DECL_OVERRIDE {return S_MINE;}
   bool isActive() {return active;}
   double getFuel() {return fuel;}
   void setFuel(double f) {fuel=(f<0.0?0.0:f);}
   void forward(double mult) Q_DECL_OVERRIDE;
   void explode();
   bool explodes() {return expl;}
   bool over() {return (expl&&(explosiontime>(timeToGo-0.1)));}
   void calculateGravity(double gravity,double mult) Q_DECL_OVERRIDE;
private:
   bool expl,active;
   double activateTime,fuel,timeToGo,explosiontime;
   QList<QString> exploframes; /* when mine explodes, we set frames to exploframes (needed because both player's mines have
   			          the same explosion animation) */
};

class ExplosionSprite:public AnimatedSprite
{
public:
   explicit ExplosionSprite(QSvgRenderer* svg, const QList<QString> &animation, MobileSprite *sp = 0);
   int type() const Q_DECL_OVERRIDE {return S_EXPLOSION;}
   bool isOver() {return over;}
   void forward(double mult) Q_DECL_OVERRIDE;
private:
   double timeToGo,time;
   bool over;
   MobileSprite *obj;
};

#endif // KSPACEDUEL_SPRITES_H
