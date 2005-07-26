#include <math.h>

#include "ai.h"
#include "mathroutines.h"
#include "options.h"

int Ai::calcFrameIncrement[Options::EnumAiDifficulty::COUNT] = {15,8,6,2};
int Ai::calcPositionNumber[Options::EnumAiDifficulty::COUNT] = {10,15,20,60};
int Ai::calcShotDirections[Options::EnumAiDifficulty::COUNT] = {4,7,10,12};
int Ai::calcCollisions[Options::EnumAiDifficulty::COUNT]     = {30,15,10,10};
int Ai::calcNextShot[Options::EnumAiDifficulty::COUNT]       = {300,200,90,60};

Ai::Ai(int pn,ShipSprite* s[2],QPtrList<BulletSprite>* b[2],
       QPtrList<MineSprite>* m[2],SConfig *c)
{
   int i;

   playerNumber=pn;
   opponentNumber=(pn+1)%2;
   cfg=c;

   for(i=0;i<2;i++)
   {
      ship[i]=s[i];
      bullets[i]=b[i];
      mines[i]=m[i];
      shipsNextPositions[i]=new QMemArray<AiSprite>
         ((int)(calcPositionNumber[Options::aiDifficulty(playerNumber)]/cfg->gamespeed));
      aiMines[i]=new QMemArray<AiSprite>(cfg->maxMines);
      mineNumber[i]=0;
   }
   myShots.setAutoDelete(true);
   objectsHitByShip.setAutoDelete(true);
   minesHitByShot.setAutoDelete(true);
}

void Ai::newRound()
{
   accelerateFramesNumber=0;
   rotateFramesNumber=0;
   shoot=false;
   score=1e10;
   
   rotation=RNONE;
   acc=false;
   bullet=false;
   mine=false;

   borderTime=-1;
   sunTime=-1;

   calculateCollisions=(int)(calcCollisions[Options::aiDifficulty(playerNumber)]
                             /cfg->gamespeed);
   waitShot=(int) rint( random.getDouble() * 
	          calcNextShot[Options::aiDifficulty(playerNumber)]
                  /cfg->gamespeed);

   myShots.clear();
   objectsHitByShip.clear();
   minesHitByShot.clear();
}

void Ai::think()
{
   setSpriteFieldSize();
   
   myShots.clear();
   borderTime=-1;
   sunTime=-1;
   score--;
   if(waitShot>0)
      waitShot--;
   
   calculateNextPositions();
   if(Options::aiDifficulty(playerNumber)!=Options::EnumAiDifficulty::Trainee)
      testForHits();
   if(waitShot<=0)
   {
      tryShots();
      shotScores();
   }
   chooseAction();


   if(rotateFramesNumber<=0)
   {
      rotation=RNONE;
      if(accelerateFramesNumber<=0)
      {
         acc=false;
         if(shoot)
         {
            bullet=true;
            shoot=false;
         }
         else 
            bullet=false;
         score=1e10;
      }
      else
      {
         acc=true;
         accelerateFramesNumber--;
      }
   }
   else
      rotateFramesNumber--;
      
}


AiSprite Ai::nextPosition(AiSprite sp,double mult)
{
   double abs_2,nx,ny,sq,eg;
   if(!sp.sun)
   {
      abs_2=sp.x*sp.x+sp.y*sp.y;
      if(abs_2<1)
         abs_2=1;
      sq=sqrt(abs_2);
      nx=sp.x/sq;
      ny=sp.y/sq;
      eg=cfg->gravity*mult;
      sp.dx-=eg*nx/abs_2;
      sp.dy-=eg*ny/abs_2;

      sp.x+=sp.dx*mult;
      sp.y+=sp.dy*mult;

      if(sp.x*sp.x+sp.y*sp.y<1600)
         sp.sun=true;
      else
      {
             //simple bounds actions
         if(sp.x>sfwidth_2)
         {
            sp.x-=sfwidth;
            sp.border=true;
         }
         else if(sp.x<-sfwidth_2)
         {
            sp.x+=sfwidth;
            sp.border=true;
         }
         if(sp.y>sfheight_2)
         {
            sp.y-=sfheight;
            sp.border=true;
         }
         else if(sp.y<-sfheight_2)
         {
            sp.y+=sfheight;
            sp.border=true;
         }
      }      
   }

   return sp; 
}

void Ai::nextPositions(AiSprite sp,QMemArray<AiSprite> *a,int frames)
{
   int i,num;
   double fmult=cfg->gamespeed*frames;

   (*a)[0]=nextPosition(sp,cfg->gamespeed);
   num=a->size();
   for(i=1;i<num;i++)
      (*a)[i]=nextPosition((*a)[i-1],fmult);
}

void Ai::calculateNextPositions()
{
   unsigned int i,j;
   MineSprite *ms;

   j=(int)(calcPositionNumber[Options::aiDifficulty(playerNumber)]/cfg->gamespeed);
   
   if(shipsNextPositions[0]->size() != j)
      for(i=0;i<2;i++)
         shipsNextPositions[i]->resize(j);
   
   for(i=0;i<2;i++)
      nextPositions(ship[i]->toAiSprite(),shipsNextPositions[i],
                    calcFrameIncrement[Options::aiDifficulty(playerNumber)]);

   if(cfg->maxMines > aiMines[0]->size())
      for(i=0;i<2;i++)
         aiMines[i]->resize(cfg->maxMines);

   for(i=0;i<2;i++)
   {
      j=0;
      ms=mines[i]->first();
      while(ms)
      {
         (*(aiMines[i]))[j]=ms->toAiSprite();
         ms=mines[i]->next();
         j++;
      }
      mineNumber[i]=j;
   }
}

void Ai::tryShots()
{
   AiSprite shot,me;
   double rot,nr,nx,ny;
   int i,f,frameIncrement,frameNum;
   Hit hit;
   Shot *goodShot;

   me=ship[playerNumber]->toAiSprite();
   rot=ship[playerNumber]->getRotation();
   
       //Each 'frameIncrement' frames a shot is tried
   frameIncrement=(int)((2*M_PI/calcShotDirections[Options::aiDifficulty(playerNumber)])
                        /cfg->rotationSpeed);
   if(frameIncrement==0)
      frameIncrement=1;
       //Number of frames needed to rotate 180 degrees
   frameNum=(int)(M_PI/(frameIncrement*cfg->rotationSpeed));

       //if too much bullets are on the playfield, no shot is tried
   if(bullets[playerNumber]->count() <
      (cfg->maxBullets+ship[playerNumber]->getBulletPowerups()))
   {
      for(f=0;f<=frameNum;f++)
      {
         if(f!=0)
            for(i=0;i<frameIncrement;i++)
               me=nextPosition(me,cfg->gamespeed);
         else
            me=nextPosition(me,cfg->gamespeed);

         if(!ship[playerNumber]->reloadsBullet(f*frameIncrement*cfg->gamespeed))
         {
            for(i=0;i<2;i++)
            {
               if((f==0)&&(i==1))
                  continue;
               if(i==0)
                  nr=rot+frameIncrement*f*cfg->rotationSpeed;
               else
                  nr=rot-frameIncrement*f*cfg->rotationSpeed;
               
               nx=cos(nr);
               ny=sin(nr);
               shot.x=me.x+nx*SHOTDIST;
               shot.y=me.y+ny*SHOTDIST;
               shot.dx=me.dx+nx*cfg->shotSpeed;
               shot.dy=me.dy+ny*cfg->shotSpeed;
               shot.sun=false;
               shot.border=false;

               hit=firstObject(shot,f*frameIncrement,
                               calcFrameIncrement[Options::aiDifficulty(playerNumber)]);
               if((hit.object!=HNOTHING) &&
                  !((hit.object==HSHIP)&&(hit.playerNumber==playerNumber)))
               {
                  goodShot=new Shot;
                  goodShot->hit=hit;
                  goodShot->rotation=(i==0?RLEFT:RRIGHT);
                  goodShot->rotationFrames=f*frameIncrement;
                  goodShot->score=1e10;
                  myShots.append(goodShot);
               }
            }
         }
      }
   }
}

Hit Ai::firstObject(AiSprite shot,int time,int frames)
{
   int optime,i,num,rtime,basetime,t,m;
   double dist,distx,disty,shiplastdist=0;
   bool shipdistgreater=true,hitfound=false;
   Hit hit={HNOTHING,0,0,0,1e10};

   basetime=time/frames;
   if((time%frames)>0)
      basetime++;
   rtime=basetime*frames-time;
   optime=shipsNextPositions[0]->size();

   num=optime-basetime;
   
   if(num>0)
   {
      for(t=0;(t<num)&&(!hitfound)&&(!shot.sun);t++)
      {
         if(t==0)
            shot=nextPosition(shot,cfg->gamespeed*rtime);
         else
            shot=nextPosition(shot,cfg->gamespeed*frames);

             //distance to other objects
         for(i=0;i<2;i++)
         {
            distx=(*(shipsNextPositions[i]))[basetime].x-shot.x;
            disty=(*(shipsNextPositions[i]))[basetime].y-shot.y;
            dist=distx*distx+disty*disty;
                //own ship
            if(i==playerNumber)
            {
               if(dist<shiplastdist)
                  shipdistgreater=false;
               if((!shipdistgreater)&&(dist<hit.distance))
               {
                  hit.object=HSHIP;
                  hit.objectNumber=0;
                  hit.playerNumber=i;
                  hit.hitTime=basetime*frames;
                  hit.distance=dist;
               }    
               shiplastdist=dist;
            }
                //other ship
            else if(dist<hit.distance)
            {
               hit.object=HSHIP;
               hit.objectNumber=0;
               hit.playerNumber=i;
               hit.hitTime=basetime*frames;
               hit.distance=dist;
            }

                //mines
            for(m=0;m<mineNumber[i];m++)
            {
               distx=(*(aiMines[i]))[m].x-shot.x;
               disty=(*(aiMines[i]))[m].y-shot.y;
               dist=distx*distx+disty*disty;
               
               if(dist<hit.distance)
               {
                  hit.object=HMINE;
                  hit.playerNumber=i;
                  hit.objectNumber=m;
                  hit.hitTime=basetime*frames;
                  hit.distance=dist;
               }
            }
         }
         if(hit.distance<100)
            hitfound=true;
         basetime++;
      }
   }

   return hit;
}
 
void Ai::testForHits()
{
   AiSprite shot;
   unsigned int i;
   int m,p;
   BulletSprite *bullet;
   Hit *h;
   Hit hit;
   bool hitfound=false;
   double distance,dx,dy;

   if(calculateCollisions>0)
   {
      calculateCollisions--;
      h=objectsHitByShip.first();
      while(h)
      {
         if(h->hitTime>0)
         {
            h->hitTime--;
            h=objectsHitByShip.next();
         }
         else
         {
            objectsHitByShip.remove();
            h=objectsHitByShip.current();
         }
      }
      h=minesHitByShot.first();
      while(h)
      {
         if(h->hitTime>0)
         {
            h->hitTime--;
            h=minesHitByShot.next();
         }
         else
         {
            minesHitByShot.remove();
            h=minesHitByShot.current();
         }
      }
   }
   else
   {
      objectsHitByShip.clear();
      minesHitByShot.clear();
      for(i=0;i<2;i++)
      {
         for(bullet=bullets[i]->first();bullet;bullet=bullets[i]->next())
         {
            shot=bullet->toAiSprite();
            hit=firstObject(shot,0,calcFrameIncrement[Options::aiDifficulty(playerNumber)]);
            if(hit.object==HMINE)
            {
               h=new Hit(hit);
               minesHitByShot.append(h);
            }
            if((hit.object==HSHIP)&&(hit.playerNumber==playerNumber))
            {
               h=new Hit(hit);
               h->object=HSHOT;
               objectsHitByShip.append(h);
            }
         }
      }

      hit.object=HNOTHING;
      hit.distance=400;
   
      for(i=0;(i<shipsNextPositions[0]->size()) &&
             !(*shipsNextPositions[playerNumber])[i].sun;i++)
      {
         if((borderTime<0) && (*shipsNextPositions[playerNumber])[i].border)
            borderTime=i*calcFrameIncrement[Options::aiDifficulty(playerNumber)];
      
         dx=(*shipsNextPositions[playerNumber])[i].x;
         dy=(*shipsNextPositions[playerNumber])[i].y;
         distance=dx*dx+dy*dy;
         if((distance<3025)&&(sunTime<0))
            sunTime=i*calcFrameIncrement[Options::aiDifficulty(playerNumber)];

         if(!hitfound)
            for(p=0;p<2;p++)
               for(m=0;m<mineNumber[p];m++)
               {
                  dx=(*shipsNextPositions[playerNumber])[i].x-(*aiMines[p])[m].x;
                  dy=(*shipsNextPositions[playerNumber])[i].y-(*aiMines[p])[m].y;
                  distance=dx*dx+dy*dy;
                  if(hit.distance>distance)
                  {
                     hit.object=HMINE;
                     hit.playerNumber=p;
                     hit.objectNumber=m;
                     hit.hitTime=i*calcFrameIncrement[Options::aiDifficulty(playerNumber)];
                     hit.distance=distance;
                     if(distance<100)
                        hitfound=true;
                  }
               }
      }
      if(hit.object!=HNOTHING)
      {
         h=new Hit(hit);
         objectsHitByShip.append(h);
      }
      calculateCollisions=(int)(calcCollisions[Options::aiDifficulty(playerNumber)]/cfg->gamespeed);
   }
}

void Ai::shotScores()
{
   Shot *s;
   Hit *h,*mh;
   bool found,foundmh;
   double dist,dx,dy,fuel;

   
   dx=(*shipsNextPositions[playerNumber])[0].x-(*shipsNextPositions[opponentNumber])[0].x;
   dy=(*shipsNextPositions[playerNumber])[0].y-(*shipsNextPositions[opponentNumber])[0].y;
   dist=dx*dx+dy*dy;

   for(s=myShots.first();s;s=myShots.next())
   {
      fuel=(100-(ship[playerNumber]->getEnergy()-cfg->shotEnergyNeed));
      s->score=fuel*fuel/10 + s->hit.distance+s->hit.hitTime;
      if(dist > (75*75))
         s->score+=waitShot*8;
      else
         s->score+=waitShot*4;

      if(s->hit.object==HMINE)
      {
         found=false;
         for(h=objectsHitByShip.first();h && !found;h=objectsHitByShip.next())
         {
            if((h->object==HMINE)&&(h->playerNumber==s->hit.playerNumber)
               &&(h->objectNumber==s->hit.objectNumber))
                   //ship will hit a mine that will be hitten by the shot
            {
               found=true;
                   //ship hits earlier then shot
               if(h->hitTime<s->hit.hitTime)
                  s->score+=1000;
               else
               {
                  foundmh=false;
                  for(mh=minesHitByShot.first();mh && !foundmh;mh=minesHitByShot.next())
                  {
                     if((mh->playerNumber==s->hit.playerNumber)
                        &&(mh->objectNumber==s->hit.objectNumber))
                            //another shot will hit the mine
                     {
                        if(mh->hitTime<s->hit.hitTime)
                           s->score+=500;
                        else
                           s->score-=300;
                     }
                  }
               }
            }
         }
         if(!found)
            s->score+=1000;
      }
   }
}

void Ai::chooseAction()
{
   double bestScore=1e10;
   Shot *bestShot=NULL,*s;
   AiSprite actualpos;
   double posangle,movephi,phiright,phileft,torotate=0,velangle;
   int framesleft,framesright;
   bool rotateAndAccelerate=false;
   Hit *nextHit=0;
   int shotHitTime;

   
   shotHitTime=1000000;
   nextHit=0;
/*   for(h=objectsHitByShip.first();h;h=objectsHitByShip.next())
      if(h->object==HSHOT)
         if(h->hitTime<shotHitTime)
         {
            nextHit=h;
            shotHitTime=h->hitTime;
            }*/
   
   if((borderTime>0) || (sunTime>0) || (nextHit))
   {   
      actualpos=ship[playerNumber]->toAiSprite();
      posangle=rectToAngle(actualpos.x,actualpos.y);
      
      movephi=rectToAngle((*shipsNextPositions[playerNumber])[0].x,
                          (*shipsNextPositions[playerNumber])[0].y) - posangle;
      
      phileft=movephi+cfg->rotationSpeed;
      phiright=movephi-cfg->rotationSpeed;

      if((borderTime>0)&& !((sunTime>0)&&(sunTime<borderTime)))
      {
         bestScore=borderTime/cfg->gamespeed;
         if(score>bestScore)
         {
            velangle=rectToAngle(actualpos.dx,actualpos.dy);
            if(fabs(difference(posangle+3*M_PI/4,velangle))
               < fabs(difference(posangle-3*M_PI/4,velangle)))
               torotate=posangle-3*M_PI/4-ship[playerNumber]->getRotation();
            else
               torotate=posangle+3*M_PI/4-ship[playerNumber]->getRotation();
            rotateAndAccelerate=true;
            score=bestScore;
            accelerateFramesNumber=(int)(8/cfg->gamespeed);
         }
      }
      else if(sunTime>0)
      {
         bestScore=sunTime/(cfg->gamespeed*10)
            +(actualpos.x*actualpos.x+actualpos.y*actualpos.y)/5000;
         if(score>bestScore)
         {
            velangle=rectToAngle(actualpos.dx,actualpos.dy);
            if(fabs(difference(posangle+2*M_PI/5,velangle))
               < fabs(difference(posangle-2*M_PI/5,velangle)))
               torotate=posangle+2*M_PI/5-ship[playerNumber]->getRotation();
            else
               torotate=posangle-2*M_PI/5-ship[playerNumber]->getRotation();
            rotateAndAccelerate=true;
            score=bestScore;
            accelerateFramesNumber=(int)(8/cfg->gamespeed);
         }
      }
      else
      {
/*         bestScore=abs(nextHit->hitTime-90)*4/cfg->gamespeed + nextHit->distance*2
            + (100-(ship[playerNumber]->getEnergy()-cfg->shotEnergyNeed))*4;
         if((score>bestScore)&&(bestScore<400))
         {
            velangle=rectToAngle(actualpos.dx,actualpos.dy);
            if(fabs(difference(posangle+2*M_PI/5,velangle))
               < fabs(difference(posangle-2*M_PI/5,velangle)))
               torotate=posangle+2*M_PI/5-ship[playerNumber]->getRotation();
            else
               torotate=posangle-2*M_PI/5-ship[playerNumber]->getRotation();
            rotateAndAccelerate=true;
            score=bestScore;
            accelerateFramesNumber=(int)(4/cfg->gamespeed);
            }*/
      }

      if(rotateAndAccelerate)
      {
         if(phileft<0)
            framesleft=1000;
         else
         {
            while(torotate<0)
               torotate+=2*M_PI;
            while(torotate>=2*M_PI)
               torotate-=2*M_PI;
            framesleft=(int)(torotate/phileft+0.5);
         }
         
         if(phiright>0)
            framesright=1000;
         else
         {
            while(torotate>0)
               torotate-=2*M_PI;
            while(torotate<=-2*M_PI)
               torotate+=2*M_PI;
            framesright=(int)(torotate/phiright+0.5);
         }
      
         if(framesright<framesleft)
         {
            rotation=RRIGHT;
            rotateFramesNumber=framesright;
         }
         else
         {
            rotation=RLEFT;
            rotateFramesNumber=framesleft;
         }
         shoot=false;
         
      }
   }
   else
   {
      
      bestShot=0;   
      for(s=myShots.first();s;s=myShots.next())
         if(s->score<bestScore)
         {
            bestScore=s->score;
            bestShot=s;
         }
      if(bestShot)
      {
         if((bestScore<score)&&(bestScore<400))
         {
            rotation=bestShot->rotation;
            rotateFramesNumber=bestShot->rotationFrames;
            accelerateFramesNumber=0;
            shoot=true;
            score=bestScore;
            calculateCollisions = 0;
            waitShot=(int) rint( random.getDouble() * 
	                         calcNextShot[Options::aiDifficulty(playerNumber)]
                                 /cfg->gamespeed);
         }
      }
   }
}

void Ai::setSpriteFieldSize()
{
   sfwidth=(double)(ship[playerNumber]->spriteFieldWidth());
   sfheight=(double)(ship[playerNumber]->spriteFieldHeight());
   sfwidth_2=sfwidth/2.0;
   sfheight_2=sfheight/2.0;
}

