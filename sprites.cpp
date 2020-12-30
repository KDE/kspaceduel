/*
    SPDX-FileCopyrightText: 1998-2001 Andreas Zehender <az@azweb.de>
    SPDX-FileCopyrightText: 2006-2008 Dirk Rathlev <dirkrathlev@gmx.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sprites.h"

#include <math.h>

#include <QGraphicsScene>
#include <QTransform>

#include "mathroutines.h"

SunSprite::SunSprite(QSvgRenderer* svg, const QString& element)
      :SimpleSprite(svg, element)
{
    setZValue(0);
}


PowerupSprite::PowerupSprite(QSvgRenderer* svg, const QString& element, int t,
                             double lifetime)
      :SimpleSprite(svg, element)
{
   time=lifetime;
   mtype=t;
}

ShipSprite::ShipSprite(QSvgRenderer* svg, const QString& element, int pn)
      :MobileSprite(svg, element,pn)
{
   hitpoints=MAX_HP;
   energy=MAX_ENERGY;
   explosion=-1;
   setZValue(-20);
   rotation=0;
   bulletPowerups=0;
   minePowerups=0;
   angle = 0;
}

void ShipSprite::setRotation(double r)
{
   rotation=r;
   if(rotation<0)
      rotation-=((int)(rotation/(2*M_PI))-1)*2*M_PI;
   if(rotation>=2*M_PI)
      rotation-=(int)(rotation/(2*M_PI))*2*M_PI;

   setTransform(QTransform::fromTranslate(center().x(),center().y()), true);
   setTransform(QTransform().rotate(-(rotation-angle)*57.3), true);
   setTransform(QTransform::fromTranslate(-center().x(),-center().y()), true);
   angle = rotation;
}

void ShipSprite::forward(double mult)
{
   MobileSprite::forward(mult);
   if(explosion>0)
   {
      explosion--;
      if(explosion==0)
         hide();
   }
   if(reloadBulletTime>0)
      reloadBulletTime-=mult;
   if(reloadMineTime>0)
      reloadMineTime-=mult;
}

void ShipSprite::forward(double mult,int fr)
{
   MobileSprite::forward(mult,fr);
   rotation=fr/ROTNUM*M_PI*2;
   if(explosion>0)
   {
      explosion--;
      if(explosion==0)
         hide();
   }
   if(reloadBulletTime>0)
      reloadBulletTime-=mult;
   if(reloadMineTime>0)
      reloadMineTime-=mult;
}

void ShipSprite::calculateGravityAndEnergy(double gravity,double sunEnergy,
                                           double mult)
{
   double nx,ny,ex,ey,abs_2,phi,sq,eg;

   if(!stopped)
   {
      ex=x()+center().x()-scene()->width()/2.0;
      ey=y()+center().y()-scene()->height()/2.0;

      abs_2=ex*ex+ey*ey;
      sq=sqrt(abs_2);

      if ( explodes() && (sq<20) )
         stopped = true;
      else
      {
         nx=ex/sq;
         ny=ey/sq;
         eg=gravity*mult;
         setVelocity(xVelocity()-eg*nx/abs_2,
                     yVelocity()-eg*ny/abs_2);
         if(hitpoints!=0)
         {
            if(energy<99.8)
            {
               phi=rectToAngle(nx,ny);
               energy+=fabs(sunEnergy*mult/(abs_2)*cos(phi+rotation));
            }
         }
      }
   }
}

void ShipSprite::rotateRight(double rotationEnergyNeed,double rotationSpeed)
{
   if(energy>rotationEnergyNeed)
   {
      energy-=rotationEnergyNeed;
      setRotation(rotation-rotationSpeed);
   }
}

void ShipSprite::rotateLeft(double rotationEnergyNeed,double rotationSpeed)
{
   if(energy>rotationEnergyNeed)
   {
      energy-=rotationEnergyNeed;
      setRotation(rotation+rotationSpeed);
   }
}

BulletSprite::BulletSprite(QSvgRenderer* svg, const QString& element, int pn,double lifetime)
      :MobileSprite(svg,element,pn)
{
   setZValue(-10);
   time=lifetime;
}

void BulletSprite::forward(double mult)
{
   MobileSprite::forward(mult);
   time-=mult;
}

void BulletSprite::forward(double mult,int fr)
{
   MobileSprite::forward(mult,fr);
   time-=mult;
}

MineSprite::MineSprite(QSvgRenderer* svg, const QList<QString>& animation, const QList<QString>& exploanimation, int pn,double atime,double f)
      :AnimatedSprite(svg, animation,pn)
{
   exploframes = exploanimation;
   activateTime=atime;
   setZValue(-25);
   fuel=f;
   explosiontime=0;
   timeToGo=0.0;
   expl=false;
   active=false;
}

void MineSprite::explode()
{
   setAnimation(exploframes);
   timeToGo=frameCount();
   expl=true;
   explosiontime=0.0;
   setZValue(-8);
   setFuel(0.0);
}

void MineSprite::forward(double mult)
{
   if(active)
   {
      if(expl)
      {
         explosiontime+=mult;
         if(explosiontime>(timeToGo-0.001))
            explosiontime=timeToGo-0.01;
         setFrame((int)explosiontime);
      }
   }
   else
   {
      activateTime-=(double)mult;
      if(activateTime<0.0)
      {
         active=true;
         setFrame(1);
      }
   }
   if(fuel<0.001)
      MobileSprite::forward(mult);
}

void MineSprite::calculateGravity(double gravity,double mult)
{
   double abs_2,nx,ny,ex,ey,sq,eg;

   if(!stopped)
   {
      ex=x()-scene()->width()/2.0;
      ey=y()-scene()->height()/2.0;

      abs_2=ex*ex+ey*ey;
      sq=sqrt(abs_2);
      nx=ex/sq;
      ny=ey/sq;
      eg=gravity*mult;
      if(fuel<0.001)
         setVelocity(xVelocity()-eg*nx/abs_2,
                     yVelocity()-eg*ny/abs_2);
      else
         fuel-=eg/abs_2;
   }
}

ExplosionSprite::ExplosionSprite(QSvgRenderer* svg, const QList<QString>& animation, MobileSprite *sp)
      :AnimatedSprite(svg, animation)
{
   over=false;
   setZValue(5);
   obj=sp;
   timeToGo = frameCount();
   time=0;

   setPos(QPointF(sp->x(),sp->y()));
}

void ExplosionSprite::forward(double mult)
{
   int of=frame();
   setPos(QPointF(obj->x(),obj->y()));
   time+=mult;

   if(time>=timeToGo)
   {
      over=true;
      hide();
   }
   else
      if((int)time!=of)
         setFrame((int)time);
}
