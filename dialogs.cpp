#include <qpushbutton.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qtabwidget.h>
#include <qslider.h>
#include <qlcdnumber.h>
#include <qcheckbox.h>
#include <qgroupbox.h>
#include <qlayout.h>
#include <qvalidator.h>
#include <qvbox.h>

#include <kmessagebox.h>
#include <kconfig.h>
#include <klocale.h>
#include <kapplication.h>
#include <kaccel.h>

#include "dialogs.h"

#ifdef kspaceduel_only_for_xgettext
   i18n("Game speed:"),i18n("Shot speed:"),i18n("Energy need:"),i18n("Max number:"),
   i18n("Damage:"),i18n("Life time:"),i18n("Reload time:"),i18n("Mine fuel:"),
   i18n("Energy need:"),i18n("Activate time:"),i18n("Damage:"),i18n("Max number:"),
   i18n("Reload time:"),i18n("Acceleration:"),i18n("Energy need:"),
   i18n("Rotation speed:"),i18n("Energy need:"),i18n("Crash damage:"),
   i18n("Sun energy:"),i18n("Gravity:"),i18n("Position X:"),i18n("Position Y:"),
   i18n("Velocity X:"),i18n("Velocity Y:"),
   i18n("Appearance time:"),i18n("Life time:"),i18n("Energy amount:"),i18n("Shield amount:")
#endif

char ConfigSetup::LabelName[EditNum][25]=
{ "Game speed:",
 "Shot speed:","Energy need:","Max number:","Damage:","Life time:","Reload time:",
 "Mine fuel:","Energy need:","Activate time:","Damage:","Max number:","Reload time:",
 "Acceleration:","Energy need:","Rotation speed:",
 "Energy need:","Crash damage:",
 "Sun energy:","Gravity:",
 "Position X:","Position Y:","Velocity X:","Velocity Y:",
  "Appearance time:","Life time:","Energy amount:","Shield amount:"};

enum ConfigSetup::Type ConfigSetup::VarType[EditNum]=
{VarFloat,
 VarFloat,VarFloat,VarInt,VarInt,VarFloat,VarFloat,
 VarFloat,VarFloat,VarFloat,VarInt,VarInt,VarFloat,
 VarFloat,VarFloat,VarFloat,
 VarFloat,VarInt,
 VarFloat,VarFloat,
 VarFloat,VarFloat,VarFloat,VarFloat,
 VarFloat,VarFloat,VarFloat,VarInt};

double ConfigSetup::EditVal[EditNum][3]=
{{0.2,4.0,1},
 {0.1,10,3},{0,99,10},{0,10,5},{1,100,20},{50,2000,100},{0,400,10},
 {5,200,65},{0,99,5},{5,100,15},{1,100,30},{0,10,3},{0,400,10},
 {0,1.0,0.2},{0,10,1.0},{0.1,4,1},
 {0,10,0.2},{0,100,50},
 {1000,30000,9000},{0,10000,2200},
 {-250,250,-130},{-180,180,-100},{-10,10,3},{-10,10,-1.7},
 {200,4000,800},{200,4000,400},{0,99,50},{0,99,30}};

int ConfigSetup::EditDiv[EditNum]=
{100,
 10,1,1,1,1,10,
 1,1,10,1,1,10,
 100,10,10,
 10,1,
 1,1,
 10,10,100,100,
 1,1,1,1};

int ConfigSetup::Parent[EditNum]=
{TabGeneral,
 TabBullet,TabBullet,TabBullet,TabBullet,TabBullet,TabBullet,
 TabMine,TabMine,TabMine,TabMine,TabMine,TabMine,
 TabShip,TabShip,TabShip,TabShip,TabShip,
 TabSun,TabSun,
 TabStart,TabStart,TabStart,TabStart,
 TabPowerups,TabPowerups,TabPowerups,TabPowerups};

int ConfigSetup::Position[EditNum]=
{0,
 0,1,2,3,4,5,
 0,1,2,3,4,5,
 0,1,2,3,4,
 0,1,
 0,1,2,3,
 0,1,2,3};

const int LCDLen=6;

ConfigSetup::ConfigSetup(SConfig *custom,SOptions *opt,
                         QWidget *parent,const char *name)
      :QWidget( parent, name )
{
   QLabel *label[EditNum];
   QGridLayout *stacklayout[TabNum];
   QWidget *configWidgets[TabNum];
   //QGroupBox *box;
   
   int i;
   
   resize(500,400);
   //setCaption(i18n("Game Setup"));
   //setHelp( "OptionsConfigurations" );
   
   customConfig=custom;
   config=*customConfig;
   gameOptions=opt;
   options=*opt;

   //box=new QGroupBox(i18n("Config"),this);
   //setMainWidget( box );
   QVBoxLayout *boxlayout = new QVBoxLayout( this, 6 );
   
   tabs=new QTabWidget(this);
   for(i=0;i<TabNum;i++)
   {
      configWidgets[i]=new QWidget(tabs);
      stacklayout[i]=new QGridLayout(configWidgets[i],6,3,10);
   }
   
   for(i=0;i<EditNum;i++)
   {
      label[i]=new QLabel(i18n(LabelName[i]),configWidgets[Parent[i]]);
      slider[i]=new QSlider(EditVal[i][0]*EditDiv[i],EditVal[i][1]*EditDiv[i],
                            (EditVal[i][1]-EditVal[i][0])/10,
                            EditVal[i][2]*EditDiv[i],
                            QSlider::Horizontal,configWidgets[Parent[i]]);
      connect(slider[i],SIGNAL(valueChanged(int)),SLOT(sliderChanged(int)));
      value[i]=new QLCDNumber(LCDLen,configWidgets[Parent[i]]);
      value[i]->setFrameStyle(QFrame::NoFrame);
   }

   configCombo=new QComboBox(false,this);
   connect(configCombo,SIGNAL(activated(int)),SLOT(configSelected(int)));
   for(i=0;i<predefinedConfigNum;i++)
      configCombo->insertItem(i18n(predefinedConfigName[i]));
   configCombo->insertItem(i18n("Custom"));

   boxlayout->addSpacing( 2 * 6 );
   boxlayout->addWidget(configCombo);
   boxlayout->addWidget(tabs);
   
   for(i=0;i<EditNum;i++)
   {
      stacklayout[Parent[i]]->addWidget(label[i],Position[i],0);
      stacklayout[Parent[i]]->addWidget(slider[i],Position[i],1);
      stacklayout[Parent[i]]->addWidget(value[i],Position[i],2);      
   }
   
   for(i=0;i<TabNum;i++)
      stacklayout[i]->activate();

   tabs->addTab(configWidgets[0],i18n("General"));
   tabs->addTab(configWidgets[1],i18n("Bullet"));
   tabs->addTab(configWidgets[2],i18n("Name","Mine"));
   tabs->addTab(configWidgets[3],i18n("Ship"));
   tabs->addTab(configWidgets[4],i18n("Sun"));
   tabs->addTab(configWidgets[5],i18n("Start"));
   tabs->addTab(configWidgets[6],i18n("Powerups"));
 
   configCombo->setCurrentItem(opt->lastConfig);
   if(opt->lastConfig==predefinedConfigNum)
   {
      displayConfig(config);
      for(i=0;i<EditNum;i++)
         slider[i]->setEnabled(true);
   }
   else
   {
      displayConfig(predefinedConfig[opt->lastConfig]);
      for(i=0;i<EditNum;i++)
         slider[i]->setEnabled(false);
   }
   //enableButton( Default, opt->lastConfig == predefinedConfigNum );
}

void ConfigSetup::valueChanged(int ednum,int value)
{
   if(options.lastConfig==predefinedConfigNum)
      switch(ednum)
      {
         case EditMaxBullets:config.maxBullets=value;break;
         case EditBulletDamage:config.bulletDamage=value;break;
         case EditMaxMines:config.maxMines=value;break;
         case EditMineDamage:config.mineDamage=value;break;
         case EditShipDamage:config.shipDamage=value;break;
         case EditPowerupShieldAmount:config.powerupShieldAmount=value;break;
      }
}

void ConfigSetup::valueChanged(int ednum,double value)
{
   if(options.lastConfig==predefinedConfigNum)
      switch(ednum)
      {
         case EditSunEnergy:config.sunEnergy=value;break;
         case EditGravity:config.gravity=value;break;
         case EditShotSpeed:config.shotSpeed=value;break;
         case EditShotEnergyNeed:config.shotEnergyNeed=value;break;
         case EditBulletLifeTime:config.bulletLifeTime=value;break;
         case EditBulletReloadTime:config.bulletReloadTime=value;break;   
         case EditMineFuel:config.mineFuel=value;break;
         case EditMineEnergyNeed:config.mineEnergyNeed=value;break;
         case EditMineActivateTime:config.mineActivateTime=value;break;
         case EditMineReloadTime:config.mineReloadTime=value;break;   
         case EditGamespeed:config.gamespeed=value;break;
         case EditPosX:config.startPosX=value;break;
         case EditPosY:config.startPosY=value;break;
         case EditVelX:config.startVelX=value;break;
         case EditVelY:config.startVelY=value;break;
         case EditAcc:config.acc=value;break;
         case EditEnergyNeed:config.energyNeed=value;break;
         case EditRotationSpeed:config.rotationSpeed=value;break;
         case EditRotationEnergyNeed:config.rotationEnergyNeed=value;break;
         case EditPowerupRefreshTime:config.powerupRefreshTime=value;break;
         case EditPowerupLifeTime:config.powerupLifeTime=value;break;
         case EditPowerupEnergyAmount:config.powerupEnergyAmount=value;break;
      }
}

void ConfigSetup::slotOk()
{
   *customConfig=config;
   *gameOptions=options;
   
   //accept();
}

void ConfigSetup::slotDefault()
{
   config=predefinedConfig[0];
   displayConfig(config);
}

void ConfigSetup::configSelected(int num)
{
   int i;
   
   if(!((options.lastConfig==predefinedConfigNum)&&(num==predefinedConfigNum)))
   {
      options.lastConfig=num;
      for(i=0;i<EditNum;i++)
         slider[i]->setEnabled(num==predefinedConfigNum);
      //enableButton( Default, num==predefinedConfigNum );
      if(num<predefinedConfigNum)
         displayConfig(predefinedConfig[num]);
      else
         displayConfig(config);
   }
}

void ConfigSetup::displayConfig(SConfig cfg)
{
   setValue(EditSunEnergy,cfg.sunEnergy);
   setValue(EditGravity,cfg.gravity);
   
   setValue(EditShotSpeed,cfg.shotSpeed);
   setValue(EditShotEnergyNeed,cfg.shotEnergyNeed);
   setValue(EditMaxBullets,cfg.maxBullets);
   setValue(EditBulletDamage,cfg.bulletDamage);
   setValue(EditBulletLifeTime,cfg.bulletLifeTime);
   setValue(EditBulletReloadTime,cfg.bulletReloadTime);
   
   setValue(EditMineFuel,cfg.mineFuel);
   setValue(EditMineEnergyNeed,cfg.mineEnergyNeed);
   setValue(EditMineActivateTime,cfg.mineActivateTime);
   setValue(EditMineDamage,cfg.mineDamage);
   setValue(EditMaxMines,cfg.maxMines);
   setValue(EditMineReloadTime,cfg.mineReloadTime);
   
   setValue(EditGamespeed,cfg.gamespeed);
   
   setValue(EditPosX,cfg.startPosX);
   setValue(EditPosY,cfg.startPosY);
   setValue(EditVelX,cfg.startVelX);
   setValue(EditVelY,cfg.startVelY);
   
   setValue(EditAcc,cfg.acc);
   setValue(EditEnergyNeed,cfg.energyNeed);
   setValue(EditRotationSpeed,cfg.rotationSpeed);
   setValue(EditRotationEnergyNeed,cfg.rotationEnergyNeed);
   setValue(EditShipDamage,cfg.shipDamage);

   setValue(EditPowerupLifeTime,cfg.powerupLifeTime);
   setValue(EditPowerupRefreshTime,cfg.powerupRefreshTime);
   setValue(EditPowerupShieldAmount,cfg.powerupShieldAmount);
   setValue(EditPowerupEnergyAmount,cfg.powerupEnergyAmount);
}

void ConfigSetup::setValue(int ednum,int val)
{
   QString str;
   str.sprintf("%*i",LCDLen,val);
   value[ednum]->display(str);
   slider[ednum]->setValue(val);
}

void ConfigSetup::setValue(int ednum,double val)
{
   QString str;
   int hval=(int)(val*EditDiv[ednum]+0.5);
   int n,h;
   
   if(EditDiv[ednum]==1)
      str.sprintf("%*i",LCDLen,hval);
   else
   {
      h=1;
      for(n=0;h<EditDiv[ednum];n++)
         h*=10;
      str.sprintf("%*.*f",LCDLen,n,val);
   }
   value[ednum]->display(str);
   slider[ednum]->setValue(hval);
}

void ConfigSetup::setValue(int ednum,unsigned val)
{
   QString str;
   str.sprintf("%*i",LCDLen,(int)val);
   value[ednum]->display(str);
   slider[ednum]->setValue((int)val);
}

void ConfigSetup::sliderChanged(int val)
{
   int i,n,h;
   QString str;
   
   for(i=0;(i<EditNum)&&(slider[i]!=sender());i++);
   if(i<EditNum)
   {
      if(EditDiv[i]==1)
         str.sprintf("%*i",LCDLen,val);
      else
      {
         h=1;
         for(n=0;h<EditDiv[i];n++)
            h*=10;
         str.sprintf("%*.*f",LCDLen,n,(double)val/EditDiv[i]);
      }
      value[i]->display(str);
      if(VarType[i]==VarFloat)
         valueChanged(i,(double)val/EditDiv[i]);
      else
         valueChanged(i,val);
   }
      
}

#include "dialogs.moc"
