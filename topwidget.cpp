/* Copyright (C) 1998-2001 Andreas Zehender <az@azweb.de>

This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <klocale.h>

#include <kstandardaction.h>
#include <ktoggleaction.h>
#include <kkeydialog.h>
#include <kaction.h>
#include <kstandardgameaction.h>
#include <kicon.h>
#include <QLayout>
//Added by qt3to4:
#include <QHBoxLayout>
#include <QBoxLayout>
#include <kstatusbar.h>

#include "topwidget.h"
#include "mainview.h"
#include "playerinfo.h"

MyTopLevelWidget::MyTopLevelWidget()
{
   initGameWidgets();
   initStatusBar( );
   initActions( );
   setupGUI( );
}

void MyTopLevelWidget::initGameWidgets( ){
   QWidget *w = new QWidget(this);

   playerinfo[0]=new PlayerInfo(0,w);
   playerinfo[1]=new PlayerInfo(1,w);
   playfield=new MyMainView(w);

   QBoxLayout *toplayout=new QHBoxLayout(w);
   toplayout->addWidget(playerinfo[0]);
   toplayout->addWidget(playfield);
   toplayout->addWidget(playerinfo[1]);
   toplayout->activate();

   playfield->setFocusPolicy(Qt::StrongFocus);
   playfield->setFocus();

   QObject::connect(playfield,SIGNAL(energy(int,int)),
                    SLOT(energy(int,int)));
   QObject::connect(playfield,SIGNAL(hitPoints(int,int)),
                    SLOT(hitPoints(int,int)));
   QObject::connect(playfield,SIGNAL(wins(int,int)),SLOT(wins(int,int)));
   QObject::connect(playfield,SIGNAL(setStatusText(const QString &,int)),
                    SLOT(setStatusText(const QString &,int)));

   setCentralWidget(w);
}

void MyTopLevelWidget::energy(int pn,int en)
{
   playerinfo[pn]->setEnergy(en);
}

void MyTopLevelWidget::hitPoints(int pn,int hp)
{
   playerinfo[pn]->setHitpoints(hp);
}

void MyTopLevelWidget::wins(int pn,int w)
{
   playerinfo[pn]->setWins(w);
}

void MyTopLevelWidget::initActions( )
{
   KStandardGameAction::quit(this, SLOT(close()), actionCollection());
   KStandardGameAction::gameNew(playfield, SLOT(newGame()), actionCollection());
   KAction* newRoundAct = new KAction( KIcon("spnewround"), i18n( "&New Round" ), actionCollection(), "new_round" );
   newRoundAct->setShortcut( Qt::CTRL + Qt::Key_R );
   connect( newRoundAct, SIGNAL( triggered(bool) ), playfield, SLOT( newRound( ) ) );

   MyMainView::pauseAction =
       KStandardGameAction::pause(playfield, SLOT(togglePause()), actionCollection());
   MyMainView::pauseAction->setChecked( false );
   KAction *gameStart = new KAction( i18n( "Start" ), actionCollection( ), "game_start" );
   connect(gameStart, SIGNAL(triggered(bool) ), playfield, SLOT( start( ) ));
   gameStart->setShortcut(GAME_START_SHORTCUT);
   playfield->addAction(gameStart);

   KStandardAction::preferences(playfield, SLOT(gameSetup()), actionCollection());

   // Default keys
#ifdef __GNUC__
#warning assuming this is not necessary anymore
#endif
   // actionCollection()->setAutoConnectShortcuts(false);
   KAction* ac;
   ac = new KAction(i18n("Player 1 Rotate Left"), actionCollection(), "P1KeyLeft");
   ac->setShortcut(Qt::Key_S);
   ac->setEnabled( false );
   ac = new KAction(i18n("Player 1 Rotate Right"), actionCollection(), "P1KeyRight");
   ac->setShortcut(Qt::Key_F);
   ac->setEnabled( false );
   ac = new KAction(i18n("Player 1 Accelerate"), actionCollection(), "P1KeyAcc");
   ac->setShortcut(Qt::Key_E);
   ac->setEnabled( false );
   ac = new KAction(i18n("Player 1 Shot"), actionCollection(), "P1Shot");
   ac->setShortcut(Qt::Key_D);
   ac->setEnabled( false );
   ac = new KAction(i18n("Player 1 Mine"), actionCollection(), "P1Mine");
   ac->setShortcut(Qt::Key_A);
   ac->setEnabled( false );

   ac = new KAction(i18n("Player 2 Rotate Left"), actionCollection(), "P2KeyLeft");
   ac->setShortcut(Qt::Key_Left);
   ac->setEnabled( false );
   ac = new KAction(i18n("Player 2 Rotate Right"), actionCollection(), "P2KeyRight");
   ac->setShortcut(Qt::Key_Right);
   ac->setEnabled( false );
   ac = new KAction(i18n("Player 2 Accelerate"), actionCollection(), "P2KeyAcc");
   ac->setShortcut(Qt::Key_Up);
   ac->setEnabled( false );
   ac = new KAction(i18n("Player 2 Shot"), actionCollection(), "P2Shot");
   ac->setShortcut(Qt::Key_Down);
   ac->setEnabled( false );
   ac = new KAction(i18n("Player 2 Mine"), actionCollection(), "P2Mine");
   ac->setShortcut(Qt::Key_Insert);
   ac->setEnabled( false );

   // actionCollection()->setAutoConnectShortcuts(true);
   playfield->setActionCollection(actionCollection());
}

void MyTopLevelWidget::initStatusBar( )
{
   statusBar( )->insertItem(i18n(" paused "),IDS_PAUSE,1);
   statusBar( )->insertItem("   ",IDS_MAIN,1);
   statusBar( )->insertItem("",42,2);
}

void MyTopLevelWidget::start()
{
   playfield->newGame();
   playfield->newRound();
}

void MyTopLevelWidget::setStatusText(const QString & str,int id)
{
   statusBar( )->changeItem(str,id);
}

void MyTopLevelWidget::keySetup()
{
   playfield->pause();
   KKeyDialog::configure( actionCollection( ), KKeyChooser::LetterShortcutsAllowed, this, true );
}

#include "topwidget.moc"
