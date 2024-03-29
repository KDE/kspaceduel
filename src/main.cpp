/*
    SPDX-FileCopyrightText: 1998-2001 Andreas Zehender <az@azweb.de>

    SPDX-License-Identifier: GPL-2.0-or-later
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
    QApplication app(argc, argv);

    KLocalizedString::setApplicationDomain(QByteArrayLiteral("kspaceduel"));

    KAboutData aboutData( QStringLiteral("kspaceduel"), i18n("KSpaceDuel"), 
            QStringLiteral(KSPACEDUEL_VERSION_STRING),
            i18n("Space Arcade Game"),
            KAboutLicense::GPL,
            i18n("(c) 1998-2001, Andreas Zehender"),
            QString(),
            QStringLiteral("https://apps.kde.org/kspaceduel") );
    aboutData.addAuthor(i18n("Andreas Zehender"),i18n("Original Program"), QStringLiteral("az@azweb.de"));
    aboutData.addAuthor(i18n("Branan Riley"),i18n("SVG Renderer"), QStringLiteral("branan@gmail.com"));

    KAboutData::setApplicationData(aboutData);
    QApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("kspaceduel")));

    KCrash::initialize();

    QCommandLineParser parser;
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    KDBusService service;

    MyTopLevelWidget *top = new MyTopLevelWidget;
    top->show();
    top->start();

    return app.exec();
}
