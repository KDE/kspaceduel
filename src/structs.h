/*
    SPDX-FileCopyrightText: 1998-2001 Andreas Zehender <az@azweb.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef __SP_STRUCTS_H
#define __SP_STRUCTS_H

#include <KLocalizedString>

struct SConfig
{
   double gamespeed,gravity,acc,energyNeed,sunEnergy,rotationSpeed,
      mineActivateTime,mineFuel,shotSpeed,shotEnergyNeed,mineEnergyNeed,
      rotationEnergyNeed,startPosX,startPosY,startVelX,startVelY,
      bulletLifeTime,mineReloadTime,bulletReloadTime;
   unsigned bulletDamage,shipDamage,mineDamage,maxBullets,maxMines;
   double powerupLifeTime, powerupRefreshTime;
   double powerupEnergyAmount;
   unsigned powerupShieldAmount;
};

bool operator!=(const SConfig &s1, const SConfig &s2);

#define predefinedConfigNum 4
const SConfig predefinedConfig[]={{1.0, 2200.0, 0.2, 1.0, 9000.0, 1.0,
                                   15.0, 65.0, 3.0, 10.0, 5.0,
                                   0.2, -130.0, -100.0, 3.0, -1.7,
                                   500.0,10.0,10.0,
                                   20, 50, 30, 5, 3,
                                   400.0, 800.0, 50, 30},
                                  {1.0, 2200.0, 0.2, 1.0, 9000.0, 1.0,
                                   15.0, 40.0, 5.0, 20.0, 10.0,
                                   0.2, -50.0, -150.0, 3.5, 0.9,
                                   500.0,10.0,10.0,
                                   20, 50, 30, 6, 2,
                                   400.0, 800.0, 50, 30},
                                  {1.3, 2200.0, 0.2, 1.0, 13000.0, 1.0,
                                   15.0, 50.0, 4.0, 10.0, 10.0,
                                   0.2, -50.0, -150.0, 3.2, -0.9,
                                   400.0,10.0,10.0,
                                   20, 50, 30, 7, 5,
                                   400.0, 800.0, 50, 30},
                                  {1.0, 2200.0, 0.2, 1.0, 9000.0, 1.0,
                                   15.0, 40.0, 5.0, 60.0, 50.0,
                                   0.4, -50.0, -170.0, 3.0, -0.5,
                                   500.0,10.0,10.0,
                                   20, 50, 30, 5, 3,
                                   400.0, 800.0, 50, 30}};
const char predefinedConfigName[predefinedConfigNum][15]=
{I18N_NOOP("Default"),I18N_NOOP("Bullet"),I18N_NOOP("Chaos"),I18N_NOOP("Lack of Energy")}; 

// This is because on Solaris, sun is defined (it's the vendor)
#ifdef sun
#undef sun
#endif

struct AiSprite
{
   double x,y,dx,dy;
   bool sun, border;
};

#endif
