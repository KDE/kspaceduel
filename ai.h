#ifndef __KSD_AI_H
#define __KSD_AI_H

#include <krandomsequence.h>

#include <qpoint.h>
#include <qlist.h>
#include <qarray.h>
#include <qvector.h>
#include "sprites.h"
#include "dialogs.h"


enum Rotation {RLEFT,RNONE,RRIGHT};

struct MineHit
{
   int mineNumber,hitTime;
   double distance;
};

enum HitObject {HSHIP,HMINE,HSHOT,HNOTHING};

struct Hit
{
   HitObject object;
   int playerNumber,objectNumber,hitTime;
   double distance;      //distance^2
};

struct Shot
{
   Hit hit;
   Rotation rotation;
   int rotationFrames;
   double score;
};


class Ai
{
public:
   Ai(int pn,ShipSprite* s[2],QList<BulletSprite> *b[2],
      QList<MineSprite> *m[2],SConfig *c,SOptions *o);
   void newRound();
   void think();
   bool rotateLeft(){return rotation==RLEFT;}
   bool rotateRight(){return rotation==RRIGHT;}
   bool accelerate(){return acc;}
   bool shootBullet(){return bullet;}
   bool layMine(){return mine;}
private:
   AiSprite nextPosition(AiSprite sp,double mult);
   void nextPositions(AiSprite sp,QArray<AiSprite> *a,int frames);
   Hit firstObject(AiSprite shot,int shotframes,int frames);
   void shotScores();
   void calculateNextPositions();
   void setSpriteFieldSize();
   void testForHits();
   void tryShots();
   void chooseAction();

   SConfig *cfg;
   SOptions *opt;

   KRandomSequence random;

       //actions
   Rotation rotation;
   bool acc;
   bool bullet,mine;
       //what to do when
   int rotateFramesNumber,accelerateFramesNumber;
   bool shoot;
   double score;
       //sprites
   int playerNumber,opponentNumber;
   ShipSprite *ship[2];
   QList<BulletSprite> *bullets[2];
   QList<MineSprite> *mines[2];
   QArray<AiSprite> *shipsNextPositions[2];
   QArray<AiSprite> *aiMines[2];
   int mineNumber[2];
       //possible Hits
   QList<Shot> myShots;
   QList<Hit> objectsHitByShip;
   QList<Hit> minesHitByShot;
   int borderTime;
   int sunTime;
       //SpriteField width and height
   double sfwidth,sfheight,sfwidth_2,sfheight_2; 
       //Difficulty
   static int calcFrameIncrement[DNUM];
   static int calcPositionNumber[DNUM];
   static int calcShotDirections[DNUM];
   static int calcCollisions[DNUM];
   static int calcNextShot[DNUM];
   static double calcShotRandom[DNUM];

   int calculateCollisions;
   int waitShot;
};

#endif
