#include "sprites.h"
#include "mathroutines.h"
#include <math.h>


SunSprite::SunSprite(QwSpritePixmapSequence *seq)
      :QwSprite(seq)
{
   z(0);
}


MobileSprite::MobileSprite(QwSpritePixmapSequence *seq, int pn)
      :QwRealMobileSprite(seq)
{
   stopped=false;
   playerNumber=pn;
}

MobileSprite::MobileSprite(int pn)
      :QwRealMobileSprite()
{
   stopped=false;
   playerNumber=pn;
}

void MobileSprite::forward(double mult, int fr)
{
   if(!stopped)
      QwRealMobileSprite::forward(mult,fr);
   else
      frame(fr);
}

void MobileSprite::forward(double mult)
{
   if(!stopped)
      QwRealMobileSprite::forward(mult);
}

void MobileSprite::calculateGravity(double gravity,double mult)
{
   double abs_2,nx,ny,ex,ey,sq,eg;

   if(!stopped)
   {
      ex=exact_x()-spritefield->width()/2.0;
      ey=exact_y()-spritefield->height()/2.0;
   
      abs_2=ex*ex+ey*ey;
      sq=sqrt(abs_2);
      nx=ex/sq;
      ny=ey/sq;
      eg=gravity*mult;
      setVelocity(dX()-eg*nx/abs_2,
                  dY()-eg*ny/abs_2);
   }   
}

AiSprite MobileSprite::toAiSprite()
{
       // y direction: screen:       bottom to top
       //              mathematical: top to bottom
   AiSprite as;
   as.x=exact_x()-spritefield->width()/2.0;
   as.y=-(exact_y()-spritefield->height()/2.0);
   as.dx=dX();
   as.dy=-dY();
   as.sun=false;
   as.border=false;
   return as;
}

ShipSprite::ShipSprite(QwSpritePixmapSequence *seq,int pn)
      :MobileSprite(seq,pn)
{
   hitpoints=99;
   energy=99.9;
   explosion=-1;
   z(-20);
   rotation=0;
}

ShipSprite::ShipSprite(int pn)
      :MobileSprite(pn)
{
   hitpoints=99;
   energy=99.9;
   wins=0;
   explosion=-1;
   z(-20);
   rotation=0;
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
      frame(nf);
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
      ex=exact_x()-spritefield->width()/2.0;
      ey=exact_y()-spritefield->height()/2.0;
   
      abs_2=ex*ex+ey*ey;
      sq=sqrt(abs_2);
      nx=ex/sq;
      ny=ey/sq;
      eg=gravity*mult;
      setVelocity(dX()-eg*nx/abs_2,
                  dY()-eg*ny/abs_2);
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

BulletSprite::BulletSprite(QwSpritePixmapSequence *seq,int pn,double lifetime)
      :MobileSprite(seq,pn)
{
   z(-10);
   time=lifetime;
}

BulletSprite::BulletSprite(int pn,double lifetime)
      :MobileSprite(pn)
{
   z(-10);
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

MineSprite::MineSprite(QwSpritePixmapSequence *seq,int pn,double atime,double f)
      :MobileSprite(seq,pn)
{
   activateTime=atime;
   z(-25);
   fuel=f;
   explosiontime=0;
   timeToGo=0.0;
   expl=false;
   active=false;
}

MineSprite::MineSprite(int pn,double atime,double f)
      :MobileSprite(pn)
{
   activateTime=atime;
   z(-25);
   fuel=f;
   explosiontime=0.0;
   timeToGo=0.0;
   expl=false;
   active=false;
}

void MineSprite::explode(QwSpritePixmapSequence *seq)
{
   setSequence(seq);
   timeToGo=seq->frameCount();
   expl=true;
   frame(0);
   explosiontime=0.0;
   z(-8);
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
         frame((int)explosiontime);
      }
   }
   else
   {
      activateTime-=(double)mult;
      if(activateTime<0.0)
      {
         active=true;
         frame(1);
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
      ex=exact_x()-spritefield->width()/2.0;
      ey=exact_y()-spritefield->height()/2.0;
   
      abs_2=ex*ex+ey*ey;
      sq=sqrt(abs_2);
      nx=ex/sq;
      ny=ey/sq;
      eg=gravity*mult;
      if(fuel<0.001)
         setVelocity(dX()-eg*nx/abs_2,
                     dY()-eg*ny/abs_2);
      else
         fuel-=eg/abs_2;
   }
}

ExplosionSprite::ExplosionSprite(QwSpritePixmapSequence *seq,MobileSprite *sp)
      :QwRealSprite(seq)
{
   over=false;
   z(-5);
   obj=sp;
   timeToGo=seq->frameCount();
   time=0;

   moveTo(sp->exact_x(),sp->exact_y());
}

ExplosionSprite::ExplosionSprite(MobileSprite *sp)
      :QwRealSprite()
{
   over=false;
   z(-5);
   time=0;
   obj=sp;

   moveTo(sp->exact_x(),sp->exact_y());
}

void ExplosionSprite::forward(double mult)
{
   int of=frame();
   moveTo(obj->exact_x(),obj->exact_y());
   time+=mult;
   
   if(time>=timeToGo)
   {
      over=true;
      hide();
   }
   else
      if((int)time!=of)
         frame((int)time);
}

void ExplosionSprite::setSequence(QwSpritePixmapSequence *seq)
{
   timeToGo=seq->frameCount();
   QwRealSprite::setSequence(seq);
}
