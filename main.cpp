#include <kapp.h>
#include <klocale.h>
#include <kcmdlineargs.h>

#include "version.h"
#include "topwidget.h"

static const char *description = I18N_NOOP("KDE Space Game");

int main(int argc,char **argv)
{
   KCmdLineArgs::init(argc,argv,"kspaceduel", description,  KSPACEDUEL_VERSION );

   KApplication myapplication;
   MyTopLevelWidget top;
   myapplication.setMainWidget(&top);
   top.setMinimumSize(640,480);
   top.show();
   top.start();
   return myapplication.exec();
}
