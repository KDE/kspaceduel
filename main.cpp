#include <kapp.h>
#include <klocale.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>

#include "version.h"
#include "topwidget.h"

static const char *description = I18N_NOOP("KDE Space Game");

int main(int argc,char **argv)
{
   KAboutData aboutData( "kspaceduel", I18N_NOOP("KSpaceDuel"), 
      KSPACEDUEL_VERSION, description, KAboutData::License_GPL, 
      "(c) 1998, Andreas Zehender");
   aboutData.addAuthor("Andreas Zehender",0, "az@azweb.de");
   KCmdLineArgs::init( argc, argv, &aboutData );

   KApplication myapplication;
   MyTopLevelWidget top;
   myapplication.setMainWidget(&top);
   top.setMinimumSize(640,480);
   top.show();
   top.start();
   return myapplication.exec();
}
