#include <qlabel.h>
#include <qcombobox.h>
#include <qtabwidget.h>
#include <qslider.h>
#include <qlcdnumber.h>
#include <qlayout.h>

#include <klocale.h>

#include "options.h"
#include "dialogs.h"
#include "general.h"

bool operator!=(const SConfig &s1, const SConfig &s2)
{
   return
(s1.gamespeed != s2.gamespeed) ||
(s1.gravity != s2.gravity) ||
(s1.acc != s2.acc) ||
(s1.energyNeed != s2.energyNeed) ||
(s1.sunEnergy != s2.sunEnergy) ||
(s1.rotationSpeed != s2.rotationSpeed) ||
(s1.mineActivateTime != s2.mineActivateTime) ||
(s1.mineFuel != s2.mineFuel) ||
(s1.shotSpeed != s2.shotSpeed) ||
(s1.shotEnergyNeed != s2.shotEnergyNeed) ||
(s1.mineEnergyNeed != s2.mineEnergyNeed) ||
(s1.rotationEnergyNeed != s2.rotationEnergyNeed) ||
(s1.startPosX != s2.startPosX) ||
(s1.startPosY != s2.startPosY) ||
(s1.startVelX != s2.startVelX) ||
(s1.startVelY != s2.startVelY) ||
(s1.bulletLifeTime != s2.bulletLifeTime) ||
(s1.mineReloadTime != s2.mineReloadTime) ||
(s1.bulletReloadTime != s2.bulletReloadTime) ||
(s1.bulletDamage != s2.bulletDamage) ||
(s1.shipDamage != s2.shipDamage) ||
(s1.mineDamage != s2.mineDamage) ||
(s1.maxBullets != s2.maxBullets) ||
(s1.maxMines != s2.maxMines) ||
(s1.powerupLifeTime != s2.powerupLifeTime) ||
(s1.powerupRefreshTime != s2.powerupRefreshTime) ||
(s1.powerupEnergyAmount != s2.powerupEnergyAmount) ||
(s1.powerupShieldAmount != s2.powerupShieldAmount);
}


char ConfigSetup::LabelName[EditNum][25]=
{
   I18N_NOOP("Game speed:"),	I18N_NOOP("Shot speed:"),
   I18N_NOOP("Energy need:"),	I18N_NOOP("Max number:"),
   I18N_NOOP("Damage:"),	I18N_NOOP("Life time:"),
   I18N_NOOP("Reload time:"),	I18N_NOOP("Mine fuel:"),
   I18N_NOOP("Energy need:"),	I18N_NOOP("Activate time:"),
   I18N_NOOP("Damage:"),	I18N_NOOP("Max number:"),
   I18N_NOOP("Reload time:"),	I18N_NOOP("Acceleration:"),
   I18N_NOOP("Energy need:"),	I18N_NOOP("Rotation speed:"),
   I18N_NOOP("Energy need:"),	I18N_NOOP("Crash damage:"),
   I18N_NOOP("Sun energy:"),	I18N_NOOP("Gravity:"),
   I18N_NOOP("Position X:"),	I18N_NOOP("Position Y:"),
   I18N_NOOP("Velocity X:"),	I18N_NOOP("Velocity Y:"),
   I18N_NOOP("Appearance time:"),I18N_NOOP("Life time:"),
   I18N_NOOP("Energy amount:"),	I18N_NOOP("Shield amount:")
};

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

ConfigSetup::ConfigSetup(SConfig *custom,QWidget *parent,const char *name)
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

   customConfig=custom;

   updateWidgets();
}

void ConfigSetup::updateWidgets()
{
   config=*customConfig;
   selectedConfig = -1;
   configCombo->setCurrentItem(Options::lastConfig());
   configSelected(Options::lastConfig());
}

void ConfigSetup::valueChanged(int ednum,int value)
{
   if(selectedConfig==predefinedConfigNum)
   {
      switch(ednum)
      {
         case EditMaxBullets:config.maxBullets=value;break;
         case EditBulletDamage:config.bulletDamage=value;break;
         case EditMaxMines:config.maxMines=value;break;
         case EditMineDamage:config.mineDamage=value;break;
         case EditShipDamage:config.shipDamage=value;break;
         case EditPowerupShieldAmount:config.powerupShieldAmount=value;break;
      }
      emit changed();
   }
}

void ConfigSetup::valueChanged(int ednum,double value)
{
   if(selectedConfig==predefinedConfigNum)
   {
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
      emit changed();
   }
}

void ConfigSetup::updateSettings()
{
   *customConfig=config;

   Options::setLastConfig(selectedConfig);
   Options::writeConfig();
}

bool ConfigSetup::hasChanged()
{
   if (configCombo->currentItem() != Options::lastConfig())
      return true;

   if (configCombo->currentItem() != predefinedConfigNum)
      return false;

   return ((*customConfig) != config);
}

void ConfigSetup::updateWidgetsDefault()
{
   configCombo->setCurrentItem(0);
   configSelected(0);
}

bool ConfigSetup::isDefault()
{
   return configCombo->currentItem() == 0;
}

void ConfigSetup::configSelected(int num)
{
   int i;
   if(!((selectedConfig==predefinedConfigNum)&&(num==predefinedConfigNum)))
   {
      selectedConfig = num;
      for(i=0;i<EditNum;i++)
         slider[i]->setEnabled(num==predefinedConfigNum);
      //enableButton( Default, num==predefinedConfigNum );
      if(num<predefinedConfigNum)
         displayConfig(predefinedConfig[num]);
      else
         displayConfig(config);
      emit changed();
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

SettingsDialog::SettingsDialog(SConfig *customConfig, QWidget *parent, const char *name)
  : KConfigDialog( parent, name, Options::self())
{
  General *general = new General();
  addPage(general, i18n("General"), "package_settings", i18n("General Settings"));

  cs = new ConfigSetup(customConfig);
  addPage(cs, i18n("Game"), "kspaceduel", i18n("Game Settings"));
  connect(cs, SIGNAL(changed()), this, SLOT(updateButtons()));
 
//  resize(600,400);
}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::updateWidgetsDefault()
{
   cs->updateWidgetsDefault();
}

void SettingsDialog::updateWidgets()
{
  cs->updateWidgets();
}

void SettingsDialog::updateSettings()
{
   cs->updateSettings();
   emit settingsUpdated();
}
 
bool SettingsDialog::hasChanged()
{
   return cs->hasChanged();
}

bool SettingsDialog::isDefault()
{
   return cs->isDefault();
}

#include "dialogs.moc"
