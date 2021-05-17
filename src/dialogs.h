/*
    SPDX-FileCopyrightText: 1998-2001 Andreas Zehender <az@azweb.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KSPACEDUEL_DIALOGS_H
#define KSPACEDUEL_DIALOGS_H

class QComboBox;
class QTabWidget;
class QSlider;
class QLCDNumber;

#include <KConfigDialog>

#include "defines.h"
#include "structs.h"


class ConfigSetup:public QWidget
{
   Q_OBJECT
public:
   explicit ConfigSetup(SConfig *custom,QWidget *parent = nullptr);

   bool hasChanged();
   bool isDefault();
   void updateSettings();
   void updateWidgets();
   void updateWidgetsDefault();

Q_SIGNALS:
   void changed();

protected Q_SLOTS:
   void configSelected(int num);
   void sliderChanged(int val);
protected:   
   void valueChanged(int ednum,int val);
   void valueChanged(int ednum,double val);
   void displayConfig(const SConfig &cfg);
   void setValue(int ednum,int val);
   void setValue(int ednum,double val);
   void setValue(int ednum,unsigned val);
private:
   enum {EditGamespeed=0,
         EditShotSpeed,EditShotEnergyNeed,EditMaxBullets,EditBulletDamage,
         EditBulletLifeTime,EditBulletReloadTime,
         EditMineFuel,EditMineEnergyNeed,EditMineActivateTime,
         EditMineDamage,EditMaxMines,EditMineReloadTime,
         EditAcc,EditEnergyNeed,EditRotationSpeed,
         EditRotationEnergyNeed,EditShipDamage,
         EditSunEnergy,EditGravity,
         EditPosX,EditPosY,EditVelX,EditVelY,
         EditPowerupRefreshTime,EditPowerupLifeTime,
         EditPowerupEnergyAmount,EditPowerupShieldAmount,
         EditNum};
   enum {TabGeneral=0,TabBullet,TabMine,
         TabShip,TabSun,TabStart,TabPowerups,TabNum};
   enum Type {VarInt,VarFloat};
   
   static char LabelName[EditNum][25];
   static int Parent[EditNum];
   static int Position[EditNum];
   static const char *TabName[TabNum];
   static double EditVal[EditNum][3];
   static int EditDiv[EditNum];
   static Type VarType[EditNum];
 
   QTabWidget *tabs;

   QSlider *slider[EditNum];
   QLCDNumber *value[EditNum];
   
   QComboBox *configCombo;
   SConfig *customConfig,config;
   
   int selectedConfig;
};

class SettingsDialog : public KConfigDialog
{
        Q_OBJECT
public:
        explicit SettingsDialog(SConfig *customConfig, QWidget *parent = nullptr, const char *name = nullptr);
        ~SettingsDialog() override;

Q_SIGNALS:
        void settingsUpdated();

private Q_SLOTS:
        void updateWidgets() override;
        void updateWidgetsDefault() override;
        void updateSettings() override;
                                        
private:
        bool hasChanged() override;
        bool isDefault() override;

private:
        ConfigSetup* cs;
};

#endif // KSPACEDUEL_DIALOGS_H
