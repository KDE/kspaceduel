#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>

#include "version.h"
#include "topwidget.h"

static const char description[] = I18N_NOOP("KDE Space Game");

int main(int argc,char **argv)
{
   KAboutData aboutData( "kspaceduel", I18N_NOOP("KSpaceDuel"), 
      KSPACEDUEL_VERSION, description, KAboutData::License_GPL, 
      "(c) 1998-2000, Andreas Zehender");
   aboutData.addAuthor("Andreas Zehender",0, "az@azweb.de");
   KCmdLineArgs::init( argc, argv, &aboutData );

   KApplication myapplication;
   MyTopLevelWidget* top = new MyTopLevelWidget( );
   myapplication.setMainWidget(top);
   top->show();
   top->start();
   return myapplication.exec();
}

#ifdef kspaceduel_only_for_xgettext
i18n( "Default" ), i18n( "Bullet" ), i18n( "Chaos" ), i18n( "Lack of energy" )
#endif     

