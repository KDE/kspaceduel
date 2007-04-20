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

#include "topwidget.h"

#include <QLayout>

#include <kicon.h>
#include <kshortcutsdialog.h>
#include <klocale.h>
#include <kstandardaction.h>
#include <kstandardgameaction.h>
#include <kstatusbar.h>
#include <ktoggleaction.h>
#include <kactioncollection.h>

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
   QAction* ac;

   ac = KStandardGameAction::quit(this, SLOT(close()), this);
   actionCollection()->addAction(ac->objectName(), ac);
   ac = KStandardGameAction::gameNew(playfield, SLOT(newGame()), this);
   actionCollection()->addAction(ac->objectName(), ac);
   QAction* newRoundAct = actionCollection()->addAction( "new_round" );
   newRoundAct->setIcon( KIcon("spnewround") );
   newRoundAct->setText( i18n( "&New Round" ) );
   newRoundAct->setShortcut( Qt::CTRL + Qt::Key_R );
   connect( newRoundAct, SIGNAL( triggered(bool) ), playfield, SLOT( newRound( ) ) );

   MyMainView::pauseAction =
       KStandardGameAction::pause(playfield, SLOT(togglePause()), this);
   actionCollection()->addAction(MyMainView::pauseAction->objectName(), MyMainView::pauseAction);
   MyMainView::pauseAction->setChecked( false );
   QAction *gameStart = actionCollection()->addAction( "game_start" );
   gameStart->setText( i18n( "Start" ) );
   connect(gameStart, SIGNAL(triggered(bool) ), playfield, SLOT( start( ) ));
   gameStart->setShortcut(GAME_START_SHORTCUT);
   playfield->addAction(gameStart);

   ac = KStandardAction::preferences(playfield, SLOT(gameSetup()), this);
   actionCollection()->addAction(ac->objectName(), ac);

   // Default keys
#ifdef __GNUC__
#warning assuming this is not necessary anymore
#endif
   // actionCollection()->setAutoConnectShortcuts(false);
   ac = actionCollection()->addAction("P1KeyLeft");
   ac->setText(i18n("Player 1 Rotate Left"));
   ac->setShortcut(Qt::Key_S);
   ac->setEnabled( false );
   ac = actionCollection()->addAction("P1KeyRight");
   ac->setText(i18n("Player 1 Rotate Right"));
   ac->setShortcut(Qt::Key_F);
   ac->setEnabled( false );
   ac = actionCollection()->addAction("P1KeyAcc");
   ac->setText(i18n("Player 1 Accelerate"));
   ac->setShortcut(Qt::Key_E);
   ac->setEnabled( false );
   ac = actionCollection()->addAction("P1Shot");
   ac->setText(i18n("Player 1 Shot"));
   ac->setShortcut(Qt::Key_D);
   ac->setEnabled( false );
   ac = actionCollection()->addAction("P1Mine");
   ac->setText(i18n("Player 1 Mine"));
   ac->setShortcut(Qt::Key_A);
   ac->setEnabled( false );

   ac = actionCollection()->addAction("P2KeyLeft");
   ac->setText(i18n("Player 2 Rotate Left"));
   ac->setShortcut(Qt::Key_Left);
   ac->setEnabled( false );
   ac = actionCollection()->addAction("P2KeyRight");
   ac->setText(i18n("Player 2 Rotate Right"));
   ac->setShortcut(Qt::Key_Right);
   ac->setEnabled( false );
   ac = actionCollection()->addAction("P2KeyAcc");
   ac->setText(i18n("Player 2 Accelerate"));
   ac->setShortcut(Qt::Key_Up);
   ac->setEnabled( false );
   ac = actionCollection()->addAction("P2Shot");
   ac->setText(i18n("Player 2 Shot"));
   ac->setShortcut(Qt::Key_Down);
   ac->setEnabled( false );
   ac = actionCollection()->addAction("P2Mine");
   ac->setText(i18n("Player 2 Mine"));
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
}

void MyTopLevelWidget::setStatusText(const QString & str,int id)
{
   statusBar( )->changeItem(str,id);
}

void MyTopLevelWidget::keySetup()
{
   playfield->pause();
   KShortcutsDialog::configure( actionCollection( ), KShortcutsEditor::LetterShortcutsAllowed, this, true );
}

#include "topwidget.moc"
