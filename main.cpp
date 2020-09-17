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

#include "kspaceduel_version.h"
#include "topwidget.h"

int main(int argc,char **argv)
{
    // Fixes blurry icons with fractional scaling
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication app(argc, argv);

    KLocalizedString::setApplicationDomain("kspaceduel");

    KAboutData aboutData( QStringLiteral("kspaceduel"), i18n("KSpaceDuel"), 
            QStringLiteral(KSPACEDUEL_VERSION_STRING),
            i18n("Space Arcade Game"),
            KAboutLicense::GPL,
            i18n("(c) 1998-2001, Andreas Zehender"),
            QString(),
            QStringLiteral("https://kde.org/applications/games/org.kde.kspaceduel") );
    aboutData.addAuthor(i18n("Andreas Zehender"),i18n("Original Program"), QStringLiteral("az@azweb.de"));
    aboutData.addAuthor(i18n("Branan Riley"),i18n("SVG Renderer"), QStringLiteral("branan@gmail.com"));
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

