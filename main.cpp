#include <kapp.h>
#include "topwidget.h"

int main(int argc,char **argv)
{
   KApplication myapplication(argc,argv,"kspaceduel");
   MyTopLevelWidget top;
   myapplication.setMainWidget(&top);
   top.setMinimumSize(640,480);
   top.show();
   top.start();
   return myapplication.exec();
}
