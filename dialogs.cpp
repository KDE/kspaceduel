#include <qpushbutton.h>
#include <qlabel.h>
#include <qcombo.h>
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
#include <kapp.h>
#include <kaccel.h>

#include "dialogs.h"

KeySetup::KeySetup(SOptions *opt,QWidget *parent,const char *name)
  : KDialogBase( parent, name, true, i18n( "Key Setup" ), Ok|Cancel|Default|Help )
{
   int i,p;
   QWidget* topWidget = new QWidget( this );
   setMainWidget( topWidget );

   waitForKey=-1;
   options=opt;

   for( p = 0; p < 2; p++ )
     for( i = 0; i < PlayerKeyNum; i++ )
       key[p][i] = opt->playerKey[p][i];

   QVBoxLayout *topLayout = new QVBoxLayout( topWidget, 0, spacingHint( ) );

   QGroupBox *bplayer[2];
   bplayer[0]=new QGroupBox(i18n("Red Player"), topWidget );
   bplayer[1]=new QGroupBox(i18n("Blue Player"), topWidget );
 
   for( p = 0; p < 2; p++ )
   {
      button[p][PlayerKeyLeft]=new QPushButton(i18n("Rotate Left"),bplayer[p]);
      button[p][PlayerKeyRight]=new QPushButton(i18n("Rotate Right"),bplayer[p]);
      button[p][PlayerKeyAcc]=new QPushButton(i18n("Accelerate"),bplayer[p]);
      button[p][PlayerKeyShot]=new QPushButton(i18n("Shot"),bplayer[p]);
      button[p][PlayerKeyMine]=new QPushButton(i18n("Mine"),bplayer[p]);
      for(i=0;i<PlayerKeyNum;i++)
      {
         keyName[p][i]=new QLabel(KAccel::keyToString((unsigned)key[p][i]),bplayer[p]);
         keyName[p][i]->setLineWidth(1);
         keyName[p][i]->setFrameStyle(QFrame::Sunken|QFrame::Panel);
         keyName[p][i]->setMinimumSize( 80, 10 );
      }
   }

   QGridLayout *grid[2];
   for(p=0;p<2;p++)
   {
      grid[p]=new QGridLayout(bplayer[p],PlayerKeyNum+1,2,10);
      grid[p]->addRowSpacing( 0, spacingHint( ) );
      grid[p]->setRowStretch(0,0);
      for(i=0;i<PlayerKeyNum;i++)
      {
         button[p][i]->setToggleButton(true);
         grid[p]->addWidget(button[p][i],i+1,0);
         grid[p]->addWidget(keyName[p][i],i+1,1);
         grid[p]->setRowStretch(i+1,1);
      }
      grid[p]->activate();
   }

   connect(button[0][PlayerKeyLeft],SIGNAL(clicked()),SLOT(keyLeft1()));
   connect(button[0][PlayerKeyRight],SIGNAL(clicked()),SLOT(keyRight1()));
   connect(button[0][PlayerKeyAcc],SIGNAL(clicked()),SLOT(keyAcc1()));
   connect(button[0][PlayerKeyShot],SIGNAL(clicked()),SLOT(keyShot1()));
   connect(button[0][PlayerKeyMine],SIGNAL(clicked()),SLOT(keyMine1()));
   connect(button[1][PlayerKeyLeft],SIGNAL(clicked()),SLOT(keyLeft2()));
   connect(button[1][PlayerKeyRight],SIGNAL(clicked()),SLOT(keyRight2()));
   connect(button[1][PlayerKeyAcc],SIGNAL(clicked()),SLOT(keyAcc2()));
   connect(button[1][PlayerKeyShot],SIGNAL(clicked()),SLOT(keyShot2()));
   connect(button[1][PlayerKeyMine],SIGNAL(clicked()),SLOT(keyMine2()));

   QHBoxLayout *players=new QHBoxLayout( );
   topLayout->addLayout(players,PlayerKeyNum);
   players->addWidget(bplayer[0]);
   players->addWidget(bplayer[1]);

   topLayout->activate();
   setHelp( "Keys" );
}

void KeySetup::setButtons(int pl,int b)
{
   int i,p;
   for(p=0;p<2;p++)
      for(i=0;i<PlayerKeyNum;i++)
         button[p][i]->setOn((pl==p)&&(b==i));
   waitForKey=b;
   player=pl;
}

void KeySetup::slotDefault()
{
   int p,i;
   key[0][PlayerKeyLeft]=Key_S;
   key[0][PlayerKeyRight]=Key_F;
   key[0][PlayerKeyAcc]=Key_E;
   key[0][PlayerKeyShot]=Key_D;
   key[0][PlayerKeyMine]=Key_A;
   key[1][PlayerKeyLeft]=Key_Left;
   key[1][PlayerKeyRight]=Key_Right;
   key[1][PlayerKeyAcc]=Key_Up;
   key[1][PlayerKeyShot]=Key_Down;
   key[1][PlayerKeyMine]=Key_Insert;

   for(p=0;p<2;p++)
      for(i=0;i<PlayerKeyNum;i++)
         keyName[p][i]->setText(KAccel::keyToString(key[p][i]));
}

void KeySetup::slotOk()
{
   int p,i,j;
   bool sameKey=false;

   for(i=0;i<PlayerKeyNum;i++)
   {
      for(j=i+1;j<PlayerKeyNum;j++)
         sameKey=sameKey||(key[0][i]==key[0][j]);
      for(j=0;j<PlayerKeyNum;j++)
         sameKey=sameKey||(key[0][i]==key[1][j]);
   }
   for(i=0;i<PlayerKeyNum;i++)
   {
      for(j=i+1;j<PlayerKeyNum;j++)
         sameKey=sameKey||(key[1][i]==key[1][j]);
   }
   if(sameKey)
     sameKey = KMessageBox::warningContinueCancel(this,
         i18n("There are multiple functions for a key\n"
              "Continue?"),
	      i18n("Key Setup"), i18n("&OK"))
       == KMessageBox::Cancel;
   if(!sameKey)
   {
      for(p=0;p<2;p++)
         for(i=0;i<PlayerKeyNum;i++)
            options->playerKey[p][i]=key[p][i];
      
      accept();
   }
}

void KeySetup::keyLeft1()
{
   setButtons(0,PlayerKeyLeft);
}

void KeySetup::keyRight1()
{
   setButtons(0,PlayerKeyRight);
}

void KeySetup::keyAcc1()
{
   setButtons(0,PlayerKeyAcc);
}

void KeySetup::keyShot1()
{
   setButtons(0,PlayerKeyShot);
}

void KeySetup::keyMine1()
{
   setButtons(0,PlayerKeyMine);
}

void KeySetup::keyLeft2()
{
   setButtons(1,PlayerKeyLeft);
}

void KeySetup::keyRight2()
{
   setButtons(1,PlayerKeyRight);
}

void KeySetup::keyAcc2()
{
   setButtons(1,PlayerKeyAcc);
}

void KeySetup::keyShot2()
{
   setButtons(1,PlayerKeyShot);
}

void KeySetup::keyMine2()
{
   setButtons(1,PlayerKeyMine);
}

void KeySetup::keyPressEvent(QKeyEvent *ev)
{
   if(waitForKey>=0)
   {
      if(player<2)
      {
         key[player][waitForKey]=ev->key();
         keyName[player][waitForKey]->setText(KAccel::keyToString(ev->key()));
      }
      setButtons(-1,-1);
      ev->accept();
   }
   else
      QDialog::keyPressEvent(ev);
}

#ifdef kspaceduel_only_for_xgettext
   i18n("Game speed"),i18n("Shot speed"),i18n("Energy need"),i18n("Max number"),
   i18n("Damage"),i18n("Life time"),i18n("Reload time"),i18n("Mine fuel"),
   i18n("Energy need"),i18n("Activate time"),i18n("Damage"),i18n("Max number"),
   i18n("Reload time"),i18n("Acceleration"),i18n("Energy need"),
   i18n("Rotation speed"),i18n("Energy need"),i18n("Crash damage"),
   i18n("Sun energy"),i18n("Gravity"),i18n("Position X"),i18n("Position Y"),
   i18n("Velocity X"),i18n("Velocity Y"),
   i18n("Appearance time"),i18n("Life time"),i18n("Energy Amount"),i18n("Shield Amount")
#endif

char ConfigSetup::LabelName[EditNum][25]=
{ "Game speed",
 "Shot speed","Energy need","Max number","Damage","Life time","Reload time",
 "Mine fuel","Energy need","Activate time","Damage","Max number","Reload time",
 "Acceleration","Energy need","Rotation speed",
 "Energy need","Crash damage",
 "Sun energy","Gravity",
 "Position X","Position Y","Velocity X","Velocity Y",
  "Appearance time","Life time","Energy Amount","Shield Amount"};

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

#ifdef kspaceduel_only_for_xgettext
 i18n("General"),i18n("Bullet"),i18n("Mine"),i18n("Ship"),i18n("Sun"),
 i18n("Start"),i18n("Powerups")
#endif

const char *ConfigSetup::TabName[TabNum]=
{"General","Bullet","Mine","Ship","Sun","Start","Powerups"};

const int LCDLen=6;

ConfigSetup::ConfigSetup(SConfig *custom,SOptions *opt,
                         QWidget *parent,const char *name)
      :KDialogBase( parent, name, true, i18n( "Game Setup" ),
                    Ok|Cancel|Default|Help )
{
   QLabel *label[EditNum];
   QGridLayout *stacklayout[TabNum];
   QWidget *configWidgets[TabNum];
   QGroupBox *box;
   
   int i;
   
   //resize(450,400);
   //setCaption(i18n("Game Setup"));
   setHelp( "OptionsConfigurations" );
   
   customConfig=custom;
   config=*customConfig;
   gameOptions=opt;
   options=*opt;

   box=new QGroupBox(i18n("Config"),this);
   setMainWidget( box );
   QVBoxLayout *boxlayout = new QVBoxLayout( box, spacingHint( ) );
   
   tabs=new QTabWidget(box);
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

   configCombo=new QComboBox(false,box);
   connect(configCombo,SIGNAL(activated(int)),SLOT(configSelected(int)));
   for(i=0;i<predefinedConfigNum;i++)
      configCombo->insertItem(i18n(predefinedConfigName[i]));
   configCombo->insertItem(i18n("Custom"));

   boxlayout->addSpacing(9);
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

   for(i=0;i<TabNum;i++)
      tabs->addTab(configWidgets[i],i18n(TabName[i]));
   
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
   enableButton( Default, opt->lastConfig == predefinedConfigNum );
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
   
   accept();
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
      enableButton( Default, num==predefinedConfigNum );
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


#ifdef kspaceduel_only_for_xgettext
   i18n("Trainee"),i18n("Normal"),i18n("Hard"),i18n("Insane")
#endif
char AiSetup::DifficultyName[DNUM][10]={"Trainee","Normal","Hard","Insane"};

AiSetup::AiSetup(SOptions *opt,QWidget *parent,const char *name)
      :KDialogBase( parent, name, true, i18n( "Ai Setup" ), Ok|Cancel|Help )
{
   QGroupBox *playerbox[2];
   int i,j;

   options=opt;

//   resize(300,170);
//   setCaption(i18n("Ai Setup"));
   setHelp( "Ai" );
   
   QHBox* box = makeHBoxMainWidget();
   
   playerbox[0]=new QGroupBox(i18n("Red Player"),box);
   playerbox[1]=new QGroupBox(i18n("Blue Player"),box);

   QVBoxLayout *pboxlayout[2];
   QLabel *label[2];
   for(i=0;i<2;i++)
   {
      pboxlayout[i]=new QVBoxLayout(playerbox[i],10);
      pboxlayout[i]->addSpacing(10);
      
      AiCheck[i]=new QCheckBox(i18n("Player is ai"),playerbox[i]);
      AiCheck[i]->setChecked(options->isAi[i]);
      pboxlayout[i]->addWidget(AiCheck[i]);

      label[i]=new QLabel(i18n("Difficulty:"),playerbox[i]);
      pboxlayout[i]->addWidget(label[i]);
      
      AiCombo[i]=new QComboBox(false,playerbox[i]);
      for(j=0;j<DNUM;j++)
         AiCombo[i]->insertItem(i18n(DifficultyName[j]));
      AiCombo[i]->setCurrentItem(options->aiDifficulty[i]);
      pboxlayout[i]->addWidget(AiCombo[i]);
      pboxlayout[i]->activate();
   }
}

void AiSetup::slotOk()
{
   int i;
   for(i=0;i<2;i++)
   {
      options->isAi[i]=AiCheck[i]->isChecked();
      options->aiDifficulty[i]=(Difficulty)AiCombo[i]->currentItem();
   }
   accept();
}

HitpointSetup::HitpointSetup(SOptions *opt,QWidget *parent,const char *name)
      :KDialogBase( parent, name, true, i18n( "Handicap Setup" ),
                    Ok|Help|Cancel )
{
   hpred=opt->startHitPoints[0];
   hpblue=opt->startHitPoints[1];
   options=opt;

//   setCaption(i18n("Handicap Setup"));
   setHelp( "OptionsHandicap" );

   QSlider *blueSlider,*redSlider;
   QGroupBox *box=new QGroupBox(i18n("Hit points"),this);
   setMainWidget( box );

   QLCDNumber *bluelcd=new QLCDNumber(2,box);
   QLCDNumber *redlcd=new QLCDNumber(2,box);
   bluelcd->display(hpblue);
   redlcd->display(hpred);
   
   blueSlider=new QSlider(1,99,10,hpblue,QSlider::Horizontal,box);
   connect(blueSlider,SIGNAL(valueChanged(int)),SLOT(blueSliderChanged(int)));
   connect(blueSlider,SIGNAL(valueChanged(int)),bluelcd,SLOT(display(int)));
   redSlider=new QSlider(1,99,10,hpred,QSlider::Horizontal,box);
   connect(redSlider,SIGNAL(valueChanged(int)),SLOT(redSliderChanged(int)));
   connect(redSlider,SIGNAL(valueChanged(int)),redlcd,SLOT(display(int)));

   QLabel *redLabel=new QLabel(i18n("Red Player"),box);
   QLabel *blueLabel=new QLabel(i18n("Blue Player"),box);

   QVBoxLayout *boxlayout = new QVBoxLayout( box, spacingHint( ) );
   boxlayout->addSpacing(spacingHint()/*10*/);
   QGridLayout *grid=new QGridLayout(2,3);
   boxlayout->addLayout(grid);
   
   grid->addWidget(blueLabel,0,0);
   grid->addWidget(redLabel,1,0);
   grid->addWidget(blueSlider,0,1);
   grid->addWidget(redSlider,1,1);
   grid->addWidget(bluelcd,0,2);
   grid->addWidget(redlcd,1,2);
   
   boxlayout->activate();
}

void HitpointSetup::slotOk()
{
   options->startHitPoints[1]=(unsigned)hpblue;
   options->startHitPoints[0]=(unsigned)hpred;
   accept();
}

void HitpointSetup::redSliderChanged(int val)
{
   hpred=val;
}

void HitpointSetup::blueSliderChanged(int val)
{
   hpblue=val;
}


GraphicSetup::GraphicSetup(SOptions *opt,QWidget *parent,const char *name)
      :KDialogBase( parent, name, true, i18n( "Graphics Setup" ),
                    Ok|Cancel|Help|Default )
{
   options=opt;
   refreshtime=opt->refreshTime;

//   setCaption(i18n("Graphics Setup"));
   QGroupBox *box=new QGroupBox(i18n("Graphics"),this);
   setMainWidget( box );

   setHelp( "OptionsGraphics" );

   QLabel *refreshlabel=new QLabel(i18n("Refresh time"),box);
   refreshslider=new QSlider(10,100,10,refreshtime,QSlider::Horizontal,box);
   refreshnumber=new QLCDNumber(3,box);
   refreshnumber->display(refreshtime);
   connect(refreshslider,SIGNAL(valueChanged(int)),SLOT(refreshSliderChanged(int)));
   connect(refreshslider,SIGNAL(valueChanged(int)),refreshnumber,SLOT(display(int)));
   
   QVBoxLayout *boxlayout=new QVBoxLayout(box,spacingHint());
   boxlayout->addSpacing(spacingHint());
   QGridLayout *partslayout=new QGridLayout(1,3);
   boxlayout->addLayout(partslayout);
   partslayout->addWidget(refreshlabel,0,0);
   partslayout->addWidget(refreshslider,0,1);
   partslayout->addWidget(refreshnumber,0,2);
   boxlayout->activate();
}

void GraphicSetup::refreshSliderChanged(int val)
{
   refreshtime=val;
}

void GraphicSetup::slotOk()
{
   options->refreshTime=refreshtime;
   accept();
}

void GraphicSetup::slotDefault()
{
   refreshtime=33;
   refreshslider->setValue(refreshtime);
   refreshnumber->display(refreshtime);
}
