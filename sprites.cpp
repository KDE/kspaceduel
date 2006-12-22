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

#include "sprites.h"
#include "mathroutines.h"
#include <math.h>


SunSprite::SunSprite(Q3CanvasPixmapArray *seq, Q3Canvas* canvas)
      :Q3CanvasSprite(seq, canvas)
{
    // doesn't work with Qt 2.2.2 anymore
    // setZ(0);
}


PowerupSprite::PowerupSprite(Q3CanvasPixmapArray* seq, Q3Canvas* canvas, int t,
                             double lifetime)
      :Q3CanvasSprite(seq, canvas)
{
   time=lifetime;
   type=t;
}


MobileSprite::MobileSprite(Q3CanvasPixmapArray* seq, Q3Canvas* canvas, int pn)
      :Q3CanvasSprite(seq, canvas)
{
   stopped=false;
   playerNumber=pn;
}

void MobileSprite::forward(double mult, int fr)
{
   if(!stopped)
   {
      Q3CanvasSprite::moveBy(xVelocity()*mult,yVelocity()*mult);
      checkBounds();
      setFrame(fr);
   }
   else
      setFrame(fr);
}

void MobileSprite::forward(double mult)
{
   if(!stopped)
   {
      Q3CanvasSprite::moveBy(xVelocity()*mult,yVelocity()*mult);
      checkBounds();
   }
}

void MobileSprite::checkBounds()
{
   double cx, cy;
   int ch, cw;

   cx = x();
   cy = y();
   ch = canvas()->height();
   cw = canvas()->width();

   if ( (int)(cx+0.5) < 0 )
      cx = cw - 1 - fmod( -cx, cw );
   else if ( (int)(cx+0.5) > ( cw-1 ) )
      cx = fmod( cx-cw-1, cw );
   if ( (int)(cy+0.5) < 0 )
      cy = ch-1 - fmod( -cy, ch );
   else if ( (int)(cy+0.5) > ( ch-1 ) )
      cy = fmod( cy-ch-1, ch );
   if ( (cx != x()) || (cy != y()) )
   {
      // printf("%5.2f %5.2f %5.2f %5.2f\n", x(), y(), cx, cy);
      move( cx, cy );
   }
}

void MobileSprite::calculateGravity(double gravity,double mult)
{
   double abs_2,nx,ny,ex,ey,sq,eg;

   if(!stopped)
   {
      ex=x()-canvas()->width()/2.0;
      ey=y()-canvas()->height()/2.0;

      abs_2=ex*ex+ey*ey;
      sq=sqrt(abs_2);

      nx=ex/sq;
      ny=ey/sq;
      eg=gravity*mult;
      setVelocity(xVelocity()-eg*nx/abs_2,
                  yVelocity()-eg*ny/abs_2);
   }
}

AiSprite MobileSprite::toAiSprite()
{
       // y direction: screen:       bottom to top
       //              mathematical: top to bottom
   AiSprite as;
   as.x=x()-canvas()->width()/2.0;
   as.y=-(y()-canvas()->height()/2.0);
   as.dx=xVelocity();
   as.dy=-yVelocity();
   as.sun=false;
   as.border=false;
   return as;
}

ShipSprite::ShipSprite(Q3CanvasPixmapArray* seq, Q3Canvas* canvas, int pn)
      :MobileSprite(seq,canvas,pn)
{
   hitpoints=99;
   energy=99.9;
   explosion=-1;
   setZ(-20);
   rotation=0;
   bulletPowerups=0;
   minePowerups=0;
}

void ShipSprite::setRotation(double r)
{
   int nf,of=frame();
   rotation=r;
   if(rotation<0)
      rotation-=((int)(rotation/(2*M_PI))-1)*2*M_PI;
   if(rotation>=2*M_PI)
      rotation-=(int)(rotation/(2*M_PI))*2*M_PI;
   nf=(int)(rotation/(2*M_PI)*ROTNUM)%ROTNUM;
   if(nf!=of)
      setFrame(nf);
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
      ex=x()-canvas()->width()/2.0;
      ey=y()-canvas()->height()/2.0;

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

BulletSprite::BulletSprite(Q3CanvasPixmapArray* seq,Q3Canvas* canvas, int pn,double lifetime)
      :MobileSprite(seq,canvas,pn)
{
   setZ(-10);
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

MineSprite::MineSprite(Q3CanvasPixmapArray* seq, Q3Canvas* canvas, int pn,double atime,double f)
      :MobileSprite(seq,canvas,pn)
{
   activateTime=atime;
   setZ(-25);
   fuel=f;
   explosiontime=0;
   timeToGo=0.0;
   expl=false;
   active=false;
}

void MineSprite::explode(Q3CanvasPixmapArray *seq)
{
   setSequence(seq);
   timeToGo=seq->count();
   expl=true;
   setFrame(0);
   explosiontime=0.0;
   setZ(-8);
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
      ex=x()-canvas()->width()/2.0;
      ey=y()-canvas()->height()/2.0;

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

ExplosionSprite::ExplosionSprite(Q3CanvasPixmapArray* seq, Q3Canvas* canvas, MobileSprite *sp)
      :Q3CanvasSprite(seq, canvas)
{
   over=false;
   setZ(-5);
   obj=sp;
   timeToGo=seq->count();
   time=0;

   move(sp->x(),sp->y());
}

void ExplosionSprite::forward(double mult)
{
   int of=frame();
   move(obj->x(),obj->y());
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


void ExplosionSprite::setSequence(Q3CanvasPixmapArray *seq)
{
   timeToGo=seq->count();
   Q3CanvasSprite::setSequence(seq);
}
