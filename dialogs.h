#ifndef __SP_DIALOG_H
#define __SP_DIALOG_H

#include <qdialog.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qcombo.h>
#include <qtabwidget.h>
#include <qslider.h>
#include <qlcdnumber.h>
#include <qcheckbox.h>
#include <kdialogbase.h>

#include "defines.h"
#include "structs.h"


class KeySetup:public KDialogBase
{
   Q_OBJECT
public:
   KeySetup(SOptions *opt,QWidget *parent=0,const char *name=0);
protected slots:
   void slotOk();
   void slotDefault();
   void keyLeft1();
   void keyRight1();
   void keyAcc1();
   void keyShot1();
   void keyMine1();
   void keyLeft2();
   void keyRight2();
   void keyAcc2();
   void keyShot2();
   void keyMine2();
protected:
   virtual void keyPressEvent(QKeyEvent *ev);
   void setButtons(int pl,int b);
private:
   SOptions *options;
   QLabel *keyName[2][PlayerKeyNum];
   QPushButton *button[2][PlayerKeyNum];
   QPushButton *ok, *cancel, *defaults;
   int waitForKey,player,key[2][PlayerKeyNum];
};

class ConfigSetup:public KDialogBase
{
   Q_OBJECT
public:
   ConfigSetup(SConfig *custom,SOptions *opt,QWidget *parent,const char* name=0);

protected slots:
   void slotOk();
//   void helpPressed();
   void slotDefault();
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

class AiSetup : public KDialogBase
{
   Q_OBJECT
public:
   AiSetup(SOptions *opt,QWidget *parent,const char *name=0);
protected slots:
   void slotOk();
private:
   SOptions *options;
   QComboBox *AiCombo[2];
   QCheckBox *AiCheck[2];
   static char DifficultyName[DNUM][10];
};

class HitpointSetup : public KDialogBase
{
   Q_OBJECT
public:
   HitpointSetup(SOptions *opt,QWidget *parent,const char*name=0);
protected slots:
   void slotOk();
   void redSliderChanged(int);
   void blueSliderChanged(int);
private:
   int hpred,hpblue;
   SOptions *options;
};

class GraphicSetup : public KDialogBase
{
   Q_OBJECT
public:
   GraphicSetup(SOptions *opt,QWidget *parent,const char *name=0);
public slots:
   void refreshSliderChanged(int);
   void slotOk();
   void slotDefault();
private:
   int refreshtime;
   SOptions *options;
   QSlider *refreshslider;
   QLCDNumber *refreshnumber;
};
#endif
