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

#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>

#include "version.h"
#include "topwidget.h"

static const char description[] = I18N_NOOP("KDE Space Game");

int main(int argc,char **argv)
{
   KAboutData aboutData( "kspaceduel", 0, ki18n("KSpaceDuel"), 
      KSPACEDUEL_VERSION, ki18n(description), KAboutData::License_GPL, 
      ki18n("(c) 1998-2001, Andreas Zehender"));
   aboutData.addAuthor(ki18n("Andreas Zehender"),ki18n("Original Program"), "az@azweb.de");
   aboutData.addAuthor(ki18n("Branan Riley"),ki18n("SVG Renderer"), "branan@gmail.com");
   KCmdLineArgs::init( argc, argv, &aboutData );

   KApplication myapplication;
   KGlobal::locale()->insertCatalog("libkdegames");

   MyTopLevelWidget *top = new MyTopLevelWidget;
   top->show();
   top->start();
   return myapplication.exec();
}

#ifdef kspaceduel_only_for_xgettext
i18n( "Default" ), i18n( "Bullet" ), i18n( "Chaos" ), i18n( "Lack of energy" )
#endif     

