#ifndef __SP_DIALOG_H
#define __SP_DIALOG_H

class QPushButton;
class QLabel;
class QComboBox;
class QTabWidget;
class QSlider;
class QLCDNumber;
class QCheckBox;

#include <kdialogbase.h>

#include "defines.h"
#include "structs.h"

class ConfigSetup:public QWidget
{
   Q_OBJECT
public:
   ConfigSetup(SConfig *custom,SOptions *opt,QWidget *parent,const char* name=0);

public slots:
   void slotOk();
   void slotDefault();

protected slots:
   void configSelected(int num);
   void sliderChanged(int val);
protected:   
   void valueChanged(int ednum,int val);
   void valueChanged(int ednum,double val);
   void displayConfig(SConfig cfg);
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
   SOptions *gameOptions,options;
};

#endif
