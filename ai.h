#ifndef __KSD_AI_H
#define __KSD_AI_H

#include <krandomsequence.h>

#include <qptrlist.h>
#include <qmemarray.h>

#include "sprites.h"
#include "dialogs.h"
#include "options.h"

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
   Ai(int pn,ShipSprite* s[2],QPtrList<BulletSprite> *b[2],
      QPtrList<MineSprite> *m[2],SConfig *c);
   void newRound();
   void think();
   bool rotateLeft(){return rotation==RLEFT;}
   bool rotateRight(){return rotation==RRIGHT;}
   bool accelerate(){return acc;}
   bool shootBullet(){return bullet;}
   bool layMine(){return mine;}
private:
   AiSprite nextPosition(AiSprite sp,double mult);
   void nextPositions(AiSprite sp,QMemArray<AiSprite> *a,int frames);
   Hit firstObject(AiSprite shot,int shotframes,int frames);
   void shotScores();
   void calculateNextPositions();
   void setSpriteFieldSize();
   void testForHits();
   void tryShots();
   void chooseAction();

   SConfig *cfg;

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
   QPtrList<BulletSprite> *bullets[2];
   QPtrList<MineSprite> *mines[2];
   QMemArray<AiSprite> *shipsNextPositions[2];
   QMemArray<AiSprite> *aiMines[2];
   int mineNumber[2];
       //possible Hits
   QPtrList<Shot> myShots;
   QPtrList<Hit> objectsHitByShip;
   QPtrList<Hit> minesHitByShot;
   int borderTime;
   int sunTime;
       //SpriteField width and height
   double sfwidth,sfheight,sfwidth_2,sfheight_2; 
       //Difficulty
   static int calcFrameIncrement[Options::EnumAiDifficulty::COUNT];
   static int calcPositionNumber[Options::EnumAiDifficulty::COUNT];
   static int calcShotDirections[Options::EnumAiDifficulty::COUNT];
   static int calcCollisions[Options::EnumAiDifficulty::COUNT];
   static int calcNextShot[Options::EnumAiDifficulty::COUNT];
   static double calcShotRandom[Options::EnumAiDifficulty::COUNT];

   int calculateCollisions;
   int waitShot;
};

#endif
