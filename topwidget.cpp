#include <klocale.h>
#include <kapp.h>
#include <qmessagebox.h>
#include <qkeycode.h>
#include "topwidget.h"
#include "version.h"

MyTopLevelWidget::MyTopLevelWidget(const char* name)
      :KTopLevelWidget(name),
       menu(this),statusbar(this),wview(this)
{
   QPopupMenu *popup;

   popup=new QPopupMenu;
   popup->insertItem(i18n("&Quit"),this,SLOT(quit()),CTRL+Key_Q);
   menu.insertItem(i18n("&File"),popup);
   
   popup=new QPopupMenu;
   popup->insertItem(i18n("New &Game"),
                     &wview,SLOT(newGame()),CTRL+Key_G);
   popup->insertItem(i18n("&New Round"),
                     &wview,SLOT(newRound()),CTRL+Key_N);
   menu.insertItem(i18n("&Game"),popup);
   
   popup=new QPopupMenu;
   popup->insertItem(i18n("&Keys..."),&wview,SLOT(keySetup()));
   popup->insertItem(i18n("&Game..."),&wview,SLOT(gameSetup()));
   popup->insertItem(i18n("&Handicap..."),&wview,SLOT(hitpointSetup()));
   popup->insertItem(i18n("Gra&phics..."),&wview,SLOT(graphicSetup()));
                     
   popup->insertItem(i18n("&Ai..."),&wview,SLOT(aiSetup()));
   popup->insertSeparator();
   popup->insertItem(i18n("&Save Options"),
                     &wview,SLOT(writeConfig()));
   menu.insertItem(i18n("&Options"),popup);

   popup=kapp->getHelpMenu(true,QString(i18n("KSpaceduel"))
                           +" "+KSPACEDUEL_VERSION
                           + i18n("\n\nby Andreas Zehender")
                           + " (azehende@ba-stuttgart.de)");
      
   menu.insertItem(i18n("&Help"),popup);

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
