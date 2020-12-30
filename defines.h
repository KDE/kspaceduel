/*
    SPDX-FileCopyrightText: 1998-2001 Andreas Zehender <az@azweb.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KSPACEDUEL_DEFINES_H
#define KSPACEDUEL_DEFINES_H

// #include <qnamespace.h>

#define IDS_PAUSE 0
#define IDS_MAIN 1

#define PlayerKeyLeft  0
#define PlayerKeyRight 1
#define PlayerKeyAcc   2
#define PlayerKeyShot  3
#define PlayerKeyMine  4
#define PlayerKeyNum   5

#define ROTNUM 64

#define MV_BACKGROUND "sprites/backgr.png"

#define MV_SVG_FILE "sprites/default_theme.svgz"

#define MV_SHIP1 "ship_red"
#define MV_SHIP2 "ship_blue"

#define MV_BULLET1 "bullet_red"
#define MV_BULLET2 "bullet_blue"

#define MV_MINE1 "mine_red00"
#define MV_MINE2 "mine_blue00"

#define MV_SUN "sun"

#define MV_POWERBULLET "pbullet"
#define MV_POWERMINE "pmine"
#define MV_POWERSHIELD "pshield"
#define MV_POWERENERGY "penergy"

#define DEF_WIDTH 640
#define DEF_HEIGHT 480

#define EXPLOSION_TIME 7

#define S_BASE QGraphicsItem::UserType
#define S_SUN S_BASE+0
#define S_SHIP S_BASE+1
#define S_BULLET S_BASE+2
#define S_MINE S_BASE+3
#define S_EXPLOSION S_BASE+4
#define S_POWERUP S_BASE+5

#define SHOTDIST 28 //14
#define EPSILON 0.1
#define PI180 57.296f // 180/Pi

#define MAX_HP 99
#define MAX_ENERGY 99.9
#define MAX_VELOCITY 20

#define ID_EXPLOSION	1351
#define ID_MINE1	1352
#define ID_MINE2	1353
#define ID_MINEEXPLO	1354

#endif // KSPACEDUEL_DEFINES_H
