#include <klocale.h>
#include <kapp.h>
#include <kkeydialog.h>
#include <kstdaction.h>
#include <kaction.h>
#include <kconfig.h>

#include "topwidget.h"
#include "version.h"
#include "defines.h"

MyTopLevelWidget::MyTopLevelWidget(const char* name)
      :KMainWindow(0, name)
{
   wview = new DuelWidget( this );
   initActions( );
   initStatusBar( );

   setCentralWidget(wview);

   readConfig(kapp->config());
}

void MyTopLevelWidget::initActions( )
{
   KToggleAction* newAct;
   
   ( void )new KAction( i18n( "&Quit" ), "exit", CTRL + Key_Q, this,
                        SLOT( quit( ) ), actionCollection( ), "game_quit" );

   ( void )new KAction( i18n( "New &Game" ), "spnewgame", 
                        CTRL + Key_G, wview, SLOT( newGame( ) ),
                        actionCollection( ), "new_game" );
   ( void )new KAction( i18n( "&New Round" ), "spnewround",
                        CTRL + Key_N, wview, SLOT( newRound( ) ),
                        actionCollection( ), "new_round" );
   newAct = new KToggleAction( i18n( "&Pause" ), "sppausegame", 
                               CTRL + Key_P, wview, SLOT( togglePause( ) ),
                               actionCollection( ), "pause" );
   newAct->setChecked( false );
   ( void )new KAction( i18n( "Start" ), Key_Space, wview, SLOT( start( ) ),
                        actionCollection( ), "game_start" );
   
   KStdAction::keyBindings( this, SLOT( keySetup( ) ), actionCollection( ) );
   ( void )new KAction( i18n( "Player &Keys..." ), 0, wview,
                        SLOT( keySetup( ) ), actionCollection( ),
                        "options_player_keys" );
   ( void )new KAction( i18n( "&Game..." ), 0, wview, SLOT( gameSetup( ) ),
                        actionCollection( ), "options_game" );
   ( void )new KAction( i18n( "&Handicap..." ), 0, wview,
                        SLOT( hitpointSetup( ) ),
                        actionCollection( ), "options_handicap" );
   ( void )new KAction( i18n( "Gra&phics..." ), 0, wview,
                        SLOT( graphicSetup( ) ),
                        actionCollection( ), "options_graphics" );
   ( void )new KAction( i18n( "&Ai..." ), 0, wview,
                        SLOT( aiSetup( ) ),
                        actionCollection( ), "options_ai" );
   toolbarAct = KStdAction::showToolbar( this, SLOT( showToolBar( ) ),
                            actionCollection( ) );
   statusbarAct = KStdAction::showStatusbar( this, SLOT( showStatusBar( ) ),
                              actionCollection( ) );

   KStdAction::saveOptions( this, SLOT( saveOptions( ) ),
                            actionCollection( ) );

   createGUI( "kspaceduelui.rc" );

}

void MyTopLevelWidget::initStatusBar( )
{
   statusBar( )->insertItem(i18n(" paused "),IDS_PAUSE);
   statusBar( )->insertItem("   ",IDS_MAIN);

   QObject::connect(wview,SIGNAL(setStatusText(const QString &,int)),
                    SLOT(setStatusText(const QString &,int)));

}

void MyTopLevelWidget::start()
{
   wview->newGame();
   wview->newRound();
}

void MyTopLevelWidget::setStatusText(const QString & str,int id)
{
   statusBar( )->changeItem(str,id);
}

void MyTopLevelWidget::readConfig(KConfig *cfg)
{
   QSize s(640,480);
   cfg->setGroup("Graphic");
   resize(cfg->readSizeEntry("WindowSize",&s));
   wview->readConfig(cfg);
}

void MyTopLevelWidget::writeConfig(KConfig *cfg)
{
   QSize s=size();
   cfg->setGroup("Graphic");
   cfg->writeEntry("WindowSize",s);
}

void MyTopLevelWidget::quit()
{
   writeConfig(kapp->config());
   kapp->quit();
}

void MyTopLevelWidget::saveOptions()
{
   wview->writeConfig();
}

void MyTopLevelWidget::keySetup()
{
   wview->pause();
   KKeyDialog::configureKeys( actionCollection( ), "kspaceduelui.rc",
                              false, this );   
}

void MyTopLevelWidget::showToolBar( )
{
   if( toolbarAct->isChecked() )
      toolBar("mainToolBar")->show( );
   else
      toolBar("mainToolBar")->hide( );
}

void MyTopLevelWidget::showStatusBar( )
{
   if( statusbarAct->isChecked() )
      statusBar( )->show( );
   else
      statusBar( )->hide( );
}

MyTopLevelWidget::~MyTopLevelWidget( )
{
}

#include "topwidget.moc"
