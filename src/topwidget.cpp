/*
    SPDX-FileCopyrightText: 1998-2001 Andreas Zehender <az@azweb.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "topwidget.h"

#include <QBoxLayout>
#include <QIcon>
#include <QStatusBar>

#include <KActionCollection>
#include <KLocalizedString>
#include <KShortcutsDialog>
#include <KStandardAction>
#include <KGameStandardAction>

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
   toplayout->setContentsMargins(0, 0, 0, 0);
   toplayout->addWidget(playerinfo[0]);
   toplayout->addWidget(playfield);
   toplayout->addWidget(playerinfo[1]);
   toplayout->activate();

   playfield->setFocusPolicy(Qt::StrongFocus);
   playfield->setFocus();

   QObject::connect(playfield, &MyMainView::energy, this, &MyTopLevelWidget::energy);
   QObject::connect(playfield, &MyMainView::hitPoints, this, &MyTopLevelWidget::hitPoints);
   QObject::connect(playfield, &MyMainView::wins, this, &MyTopLevelWidget::wins);
   QObject::connect(playfield, &MyMainView::setStatusText, this, &MyTopLevelWidget::setStatusText);

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
   KGameStandardAction::gameNew(playfield, &MyMainView::newGame, actionCollection());
   KGameStandardAction::quit(this, &MyTopLevelWidget::close, actionCollection());

   QAction * newRoundAct = actionCollection()->addAction( QStringLiteral(  "new_round" ) );
   newRoundAct->setIcon( QIcon::fromTheme( QStringLiteral( "preferences-desktop-notification-bell" )) );
   newRoundAct->setText( i18nc("@action", "&New Round" ) );
   KActionCollection::setDefaultShortcut(newRoundAct, Qt::CTRL | Qt::Key_R);
   connect( newRoundAct, &QAction::triggered, playfield, &MyMainView::newRound );

   MyMainView::pauseAction =
       KGameStandardAction::pause(playfield, &MyMainView::togglePause, actionCollection());
   MyMainView::pauseAction->setChecked( false );
   QAction *gameStart = actionCollection()->addAction( QStringLiteral(  "game_start" ) );
   gameStart->setText( i18nc("@action start game","Start" ) );
   connect(gameStart, &QAction::triggered, playfield, &MyMainView::start);
   KActionCollection::setDefaultShortcut(gameStart, Qt::Key_Space);
   playfield->addAction(gameStart);

   KStandardAction::preferences(playfield, &MyMainView::gameSetup, actionCollection());

   // Default keys
   ac = actionCollection()->addAction( QStringLiteral( "P1KeyLeft" ));
   ac->setText(i18nc("@action", "Player 1 Rotate Left"));
   KActionCollection::setDefaultShortcut(ac, Qt::Key_S);
   ac->setEnabled( false );
   ac = actionCollection()->addAction( QStringLiteral( "P1KeyRight" ));
   ac->setText(i18nc("@action", "Player 1 Rotate Right"));
   KActionCollection::setDefaultShortcut(ac, Qt::Key_F);
   ac->setEnabled( false );
   ac = actionCollection()->addAction( QStringLiteral( "P1KeyAcc" ));
   ac->setText(i18nc("@action", "Player 1 Accelerate"));
   KActionCollection::setDefaultShortcut(ac, Qt::Key_E);
   ac->setEnabled( false );
   ac = actionCollection()->addAction( QStringLiteral( "P1Shot" ));
   ac->setText(i18nc("@action", "Player 1 Shot"));
   KActionCollection::setDefaultShortcut(ac, Qt::Key_D);
   ac->setEnabled( false );
   ac = actionCollection()->addAction( QStringLiteral( "P1Mine" ));
   ac->setText(i18nc("@action", "Player 1 Mine"));
   KActionCollection::setDefaultShortcut(ac, Qt::Key_A);
   ac->setEnabled( false );

   ac = actionCollection()->addAction( QStringLiteral( "P2KeyLeft" ));
   ac->setText(i18nc("@action", "Player 2 Rotate Left"));
   KActionCollection::setDefaultShortcut(ac, Qt::Key_Left);
   ac->setEnabled( false );
   ac = actionCollection()->addAction( QStringLiteral( "P2KeyRight" ));
   ac->setText(i18nc("@action", "Player 2 Rotate Right"));
   KActionCollection::setDefaultShortcut(ac, Qt::Key_Right);
   ac->setEnabled( false );
   ac = actionCollection()->addAction( QStringLiteral( "P2KeyAcc" ));
   ac->setText(i18nc("@action", "Player 2 Accelerate"));
   KActionCollection::setDefaultShortcut(ac, Qt::Key_Up);
   ac->setEnabled( false );
   ac = actionCollection()->addAction( QStringLiteral( "P2Shot" ));
   ac->setText(i18nc("@action", "Player 2 Shot"));
   KActionCollection::setDefaultShortcut(ac, Qt::Key_Down);
   ac->setEnabled( false );
   ac = actionCollection()->addAction( QStringLiteral( "P2Mine" ));
   ac->setText(i18nc("@action", "Player 2 Mine"));
   KActionCollection::setDefaultShortcut(ac, Qt::Key_Insert);
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
   m_statusBarLabel[IDS_MAIN]->setText(QString());
   m_statusBarLabel[2]->setText(QString());
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

   KShortcutsDialog::showDialog(actionCollection(), KShortcutsEditor::LetterShortcutsAllowed, this);
}

#include "moc_topwidget.cpp"
