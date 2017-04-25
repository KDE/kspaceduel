/*
    Copyright (C) 1998-2001 Andreas Zehender <az@azweb.de>

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

#include <QBoxLayout>
#include <QIcon>
#include <QStatusBar>

#include <KActionCollection>
#include <KLocalizedString>
#include <KShortcutsDialog>
#include <KStandardAction>
#include <KStandardGameAction>
#include <KToggleAction>

#include "mainview.h"
#include "playerinfo.h"


MyTopLevelWidget::MyTopLevelWidget()
{
   initGameWidgets();
   initStatusBar( );
   setupActions( );
   setupGUI( );
}

void MyTopLevelWidget::initGameWidgets( ){
   QWidget *w = new QWidget(this);

   PlayerInfo::loadPixmaps();
   playerinfo[0]=new PlayerInfo(0,w);
   playerinfo[1]=new PlayerInfo(1,w);
   playfield=new MyMainView(w);

   QBoxLayout *toplayout=new QHBoxLayout(w);
   toplayout->setMargin(0);
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
   QObject::connect(playfield,SIGNAL(setStatusText(QString,int)),
                    SLOT(setStatusText(QString,int)));

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

void MyTopLevelWidget::setupActions()
{
   QAction * ac;

   // Game
   KStandardGameAction::gameNew(playfield, SLOT(newGame()), actionCollection());
   KStandardGameAction::quit(this, SLOT(close()), actionCollection());

   QAction * newRoundAct = actionCollection()->addAction( QLatin1String(  "new_round" ) );
   newRoundAct->setIcon( QIcon::fromTheme( QLatin1String( "preferences-desktop-notification-bell" )) );
   newRoundAct->setText( i18n( "&New Round" ) );
   newRoundAct->setShortcut( Qt::CTRL + Qt::Key_R );
   connect( newRoundAct, SIGNAL(triggered(bool)), playfield, SLOT(newRound()) );

   MyMainView::pauseAction =
       KStandardGameAction::pause(playfield, SLOT(togglePause()), actionCollection());
   MyMainView::pauseAction->setChecked( false );
   QAction *gameStart = actionCollection()->addAction( QLatin1String(  "game_start" ) );
   gameStart->setText( i18nc( "start game","Start" ) );
   connect(gameStart, SIGNAL(triggered(bool)), playfield, SLOT(start()));
   gameStart->setShortcut(GAME_START_SHORTCUT);
   playfield->addAction(gameStart);

   KStandardAction::preferences(playfield, SLOT(gameSetup()), actionCollection());

   // Default keys
#ifdef __GNUC__
#warning assuming this is not necessary anymore
#endif
   // actionCollection()->setAutoConnectShortcuts(false);
   ac = actionCollection()->addAction( QLatin1String( "P1KeyLeft" ));
   ac->setText(i18n("Player 1 Rotate Left"));
   ac->setShortcut(Qt::Key_S);
   ac->setEnabled( false );
   ac = actionCollection()->addAction( QLatin1String( "P1KeyRight" ));
   ac->setText(i18n("Player 1 Rotate Right"));
   ac->setShortcut(Qt::Key_F);
   ac->setEnabled( false );
   ac = actionCollection()->addAction( QLatin1String( "P1KeyAcc" ));
   ac->setText(i18n("Player 1 Accelerate"));
   ac->setShortcut(Qt::Key_E);
   ac->setEnabled( false );
   ac = actionCollection()->addAction( QLatin1String( "P1Shot" ));
   ac->setText(i18n("Player 1 Shot"));
   ac->setShortcut(Qt::Key_D);
   ac->setEnabled( false );
   ac = actionCollection()->addAction( QLatin1String( "P1Mine" ));
   ac->setText(i18n("Player 1 Mine"));
   ac->setShortcut(Qt::Key_A);
   ac->setEnabled( false );

   ac = actionCollection()->addAction( QLatin1String( "P2KeyLeft" ));
   ac->setText(i18n("Player 2 Rotate Left"));
   ac->setShortcut(Qt::Key_Left);
   ac->setEnabled( false );
   ac = actionCollection()->addAction( QLatin1String( "P2KeyRight" ));
   ac->setText(i18n("Player 2 Rotate Right"));
   ac->setShortcut(Qt::Key_Right);
   ac->setEnabled( false );
   ac = actionCollection()->addAction( QLatin1String( "P2KeyAcc" ));
   ac->setText(i18n("Player 2 Accelerate"));
   ac->setShortcut(Qt::Key_Up);
   ac->setEnabled( false );
   ac = actionCollection()->addAction( QLatin1String( "P2Shot" ));
   ac->setText(i18n("Player 2 Shot"));
   ac->setShortcut(Qt::Key_Down);
   ac->setEnabled( false );
   ac = actionCollection()->addAction( QLatin1String( "P2Mine" ));
   ac->setText(i18n("Player 2 Mine"));
   ac->setShortcut(Qt::Key_Insert);
   ac->setEnabled( false );

   // actionCollection()->setAutoConnectShortcuts(true);
   playfield->setActionCollection(actionCollection());
}

void MyTopLevelWidget::initStatusBar( )
{
   for (auto &label : m_statusBarLabel)
   {
      label = new QLabel(this);
      label->setAlignment(Qt::AlignCenter);
      statusBar()->addWidget(label, 1);
   }

   m_statusBarLabel[IDS_PAUSE]->setText(i18n(" paused "));
   m_statusBarLabel[IDS_MAIN]->setText(QLatin1String());
   m_statusBarLabel[2]->setText(QLatin1String());
}

void MyTopLevelWidget::start()
{
   playfield->newGame();
}

void MyTopLevelWidget::setStatusText(const QString & str,int id)
{
   m_statusBarLabel[id]->setText(str);
}

void MyTopLevelWidget::keySetup()
{
   playfield->pause();
   KShortcutsDialog::configure( actionCollection( ), KShortcutsEditor::LetterShortcutsAllowed, this, true );
}
