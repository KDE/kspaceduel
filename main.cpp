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

#include <KAboutData>
#include <KCrash>
#include <KDBusService>
#include <KLocalizedString>

#include <QApplication>
#include <QCommandLineParser>

#include "version.h"
#include "topwidget.h"

static const char description[] = I18N_NOOP("KDE Space Game");

int main(int argc,char **argv)
{
    QApplication app(argc, argv);

    KLocalizedString::setApplicationDomain("kspaceduel");

    KAboutData aboutData( "kspaceduel", i18n("KSpaceDuel"), 
            KSPACEDUEL_VERSION, i18n(description), KAboutLicense::GPL, 
            i18n("(c) 1998-2001, Andreas Zehender"),  "http://games.kde.org/kspaceduel" );
    aboutData.addAuthor(i18n("Andreas Zehender"),i18n("Original Program"), "az@azweb.de");
    aboutData.addAuthor(i18n("Branan Riley"),i18n("SVG Renderer"), "branan@gmail.com");
    QCommandLineParser parser;
    KAboutData::setApplicationData(aboutData);
    KCrash::initialize();
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);
    KDBusService service;

    MyTopLevelWidget *top = new MyTopLevelWidget;
    top->show();
    top->start();

    app.setWindowIcon(QIcon::fromTheme(QStringLiteral("kspaceduel")));

    return app.exec();
}

