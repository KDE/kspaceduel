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

// #include <qnamespace.h>


#define IDS_PAUSE 1
#define IDS_MAIN 2

#define PlayerKeyLeft  0
#define PlayerKeyRight 1
#define PlayerKeyAcc   2
#define PlayerKeyShot  3
#define PlayerKeyMine  4
#define PlayerKeyNum   5

#define ROTNUM 64

#define MV_BACKGROUND "sprites/backgr.png"

#define MV_SHIP1_PNG "sprites/ship1/ship.png"
#define MV_SHIP2_PNG "sprites/ship2/ship.png"

#define MV_BULLET1_PNG "sprites/ship1/bullet.png"
#define MV_BULLET2_PNG "sprites/ship2/bullet.png"

#define MV_MINE1_PNG "sprites/ship1/mine1.png"
#define MV_MINE2_PNG "sprites/ship2/mine1.png"

#define MV_SUN_PNG "sprites/sun/sun.png"

#define MV_POWERBULLET_PNG "sprites/powerups/pbullet.png"
#define MV_POWERMINE_PNG "sprites/powerups/pmine.png"
#define MV_POWERSHIELD_PNG "sprites/powerups/pshield.png"
#define MV_POWERENERGY_PNG "sprites/powerups/penergy.png"

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

#define SHOTDIST 18 //14
#define EPSILON 0.1
#define PI180 57.296f // 180/Pi

#define MAX_HP 99
#define MAX_ENERGY 99.9
#define MAX_VELOCITY 20

#define GAME_START_SHORTCUT Qt::Key_Space

#define ID_EXPLOSION	1351
#define ID_MINE1	1352
#define ID_MINE2	1353
#define ID_MINEEXPLO	1354
