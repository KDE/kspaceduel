#include <klocale.h>
#include <kapp.h>
#include <qmessagebox.h>
#include <kkeydialog.h>
#include "topwidget.h"
#include "version.h"

MyTopLevelWidget::MyTopLevelWidget(const char* name)
      :KTopLevelWidget(name),
       menu(this),statusbar(this),wview(this)
{
   keys=new KAccel(this);
   KAccelMenu *popup;

   popup=new KAccelMenu(keys);
   popup->insItem(i18n("&Quit"),"&Quit",this,SLOT(quit()),KAccel::Quit);
   menu.insertItem(i18n("&File"),popup);
   
   popup=new KAccelMenu(keys);
   popup->insItem(i18n("New &Game"),"New &Game",&wview,SLOT(newGame()),"CTRL+G");
   popup->insItem(i18n("&New Round"),"&New Round",&wview,SLOT(newRound()),"CTRL+N");
   menu.insertItem(i18n("&Game"),popup);
   
   popup=new KAccelMenu(keys);
   popup->insItem(i18n("&Menu Keys..."),"&Menu Keys...",this,SLOT(keySetup()));
   popup->insItem(i18n("&Keys..."),"&Keys...",&wview,SLOT(keySetup()));
   popup->insItem(i18n("&Game..."),"&Game...",&wview,SLOT(gameSetup()));
   popup->insItem(i18n("&Handicap..."),"&Handicap...",&wview,SLOT(hitpointSetup()));
   popup->insItem(i18n("Gra&phics..."),"Gra&phics...",&wview,SLOT(graphicSetup()));
                     
   popup->insItem(i18n("&Ai..."),"&Ai...",&wview,SLOT(aiSetup()));
   popup->insertSeparator();
   popup->insItem(i18n("&Save Options"),"&Save Options",this,SLOT(saveOptions()));
   menu.insertItem(i18n("&Options"),popup);

   menu.insertItem(i18n("&Help"),kapp->getHelpMenu(true,QString(i18n("KSpaceduel"))
                                                   +" "+KSPACEDUEL_VERSION
                                                   + i18n("\n\nby Andreas Zehender")
                                                   + " (azehende@ba-stuttgart.de)"));

   keys->readSettings();

   statusbar.setInsertOrder(KStatusBar::RightToLeft);
   statusbar.setBorderWidth(2);
   statusbar.insertItem(i18n(" paused "),IDS_PAUSE);
   statusbar.insertItem("   ",IDS_MAIN);
   statusbar.setFrameStyle(QFrame::Panel|QFrame::Raised);

   QObject::connect(&wview,SIGNAL(setStatusText(const char*,int)),
                    SLOT(setStatusText(const char*,int)));

   setMenu(&menu);
   setView(&wview);
   setStatusBar(&statusbar);

   readConfig(kapp->getConfig());
}

void MyTopLevelWidget::start()
{
   wview.newGame();
   wview.newRound();
}

void MyTopLevelWidget::setStatusText(const char* str,int id)
{
   statusbar.changeItem(str,id);
}

void MyTopLevelWidget::readConfig(KConfig *cfg)
{
   QSize s(640,480);
   cfg->setGroup("Graphic");
   resize(cfg->readSizeEntry("WindowSize",&s));
   wview.readConfig(cfg);
}

void MyTopLevelWidget::writeConfig(KConfig *cfg)
{
   QSize s=size();
   cfg->setGroup("Graphic");
   cfg->writeEntry("WindowSize",s);
}

void MyTopLevelWidget::quit()
{
   writeConfig(kapp->getConfig());
   kapp->quit();
}

void MyTopLevelWidget::saveOptions()
{
   keys->writeSettings();
   wview.writeConfig();
}

void MyTopLevelWidget::keySetup()
{
   KKeyDialog::configureKeys(keys);   
}
