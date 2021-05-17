/*
    SPDX-FileCopyrightText: 1998-2001 Andreas Zehender <az@azweb.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef __KSD_AI_H
#define __KSD_AI_H

#include <QRandomGenerator>

#include "dialogs.h"
#include "options.h"
#include "sprites.h"

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
   Ai(int pn,ShipSprite* s[2],QList<BulletSprite*> *b[2],
      QList<MineSprite*> *m[2],SConfig *c);
   ~Ai();
   void newRound();
   void think();
   bool rotateLeft(){return rotation==RLEFT;}
   bool rotateRight(){return rotation==RRIGHT;}
   bool accelerate(){return acc;}
   bool shootBullet(){return bullet;}
   bool layMine(){return mine;}
private:
   AiSprite nextPosition(AiSprite sp,double mult);
   void nextPositions(const AiSprite &sp,QVector<AiSprite> *a,int frames);
   Hit firstObject(AiSprite shot,int shotframes,int frames);
   void shotScores();
   void calculateNextPositions();
   void setSpriteFieldSize();
   void testForHits();
   void tryShots();
   void chooseAction();

   SConfig *cfg;

   QRandomGenerator random;

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
   QList<BulletSprite*> *bullets[2];
   QList<MineSprite*> *mines[2];
   QVector<AiSprite> *shipsNextPositions[2];
   QVector<AiSprite> *aiMines[2];
   int mineNumber[2];
       //possible Hits
   QList<Shot*> myShots;
   QList<Hit*> objectsHitByShip;
   QList<Hit*> minesHitByShot;
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
