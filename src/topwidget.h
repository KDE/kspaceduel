/*
    SPDX-FileCopyrightText: 1998-2001 Andreas Zehender <az@azweb.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef __MY_TOP_WIDGET_H
#define __MY_TOP_WIDGET_H

#include <KXmlGuiWindow>

class PlayerInfo;
class MyMainView;
class QLabel;

class MyTopLevelWidget:public KXmlGuiWindow
{
   Q_OBJECT
public:
   MyTopLevelWidget();
   void start();

private Q_SLOTS:
   void setStatusText(const QString & text,int id);
   void keySetup();
   void energy(int pn,int en);
   void hitPoints(int pn,int hp);
   void wins(int pn,int w);

protected:
   void setupActions( );
   void initStatusBar( );
   void initGameWidgets();

private:
   PlayerInfo *playerinfo[2];
   MyMainView *playfield;

   QLabel *m_statusBarLabel[3];
};

#endif
