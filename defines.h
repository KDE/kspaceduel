#include <qnamespace.h>


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

#define MV_SHIP1_PPM "sprites/ship1/ship%02d.ppm"
#define MV_SHIP1_PBM "sprites/ship1/ship%02d.pbm"
#define MV_SHIP2_PPM "sprites/ship2/ship%02d.ppm"
#define MV_SHIP2_PBM "sprites/ship2/ship%02d.pbm"

#define MV_BULLET1_PPM "sprites/ship1/bullet.ppm"
#define MV_BULLET1_PBM "sprites/ship1/bullet.pbm"
#define MV_BULLET2_PPM "sprites/ship2/bullet.ppm"
#define MV_BULLET2_PBM "sprites/ship2/bullet.pbm"

#define MV_MINE1_PPM "sprites/ship1/mine%d.ppm"
#define MV_MINE1_PBM "sprites/ship1/mine%d.pbm"
#define MV_MINE2_PPM "sprites/ship2/mine%d.ppm"
#define MV_MINE2_PBM "sprites/ship2/mine%d.pbm"

#define MV_SUN_PPM "sprites/sun/sun.ppm"
#define MV_SUN_PBM "sprites/sun/sun.pbm"

#define MV_EXPLOSION_PPM "sprites/explosion/explos%02d.ppm"
#define MV_EXPLOSION_PBM "sprites/explosion/explos%02d.pbm"
#define MV_MINEEX_PPM "sprites/explosion/mineex%02d.ppm"
#define MV_MINEEX_PBM "sprites/explosion/mineex%02d.pbm"

#define MV_POWERBULLET_PPM "sprites/powerups/pbullet.ppm"
#define MV_POWERBULLET_PBM "sprites/powerups/pbullet.pbm"
#define MV_POWERMINE_PPM "sprites/powerups/pmine.ppm"
#define MV_POWERMINE_PBM "sprites/powerups/pmine.pbm"
#define MV_POWERSHIELD_PPM "sprites/powerups/pshield.ppm"
#define MV_POWERSHIELD_PBM "sprites/powerups/pshield.pbm"
#define MV_POWERENERGY_PPM "sprites/powerups/penergy.ppm"
#define MV_POWERENERGY_PBM "sprites/powerups/penergy.pbm"

#define DEF_WIDTH 640
#define DEF_HEIGHT 480

#define EXPLOSION_TIME 7

#define S_BASE 1500
#define S_SUN S_BASE+0
#define S_SHIP S_BASE+1
#define S_BULLET S_BASE+2
#define S_MINE S_BASE+3
#define S_EXPLOSION S_BASE+4
#define S_POWERUP S_BASE+5

#define SHOTDIST 14
#define EPSILON 0.1

#define GAME_START_SHORTCUT Qt::Key_Space
