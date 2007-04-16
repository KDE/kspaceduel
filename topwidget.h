/* Copyright (C) 1998-2001 Andreas Zehender <az@azweb.de>

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

#ifndef __MY_TOP_WIDGET_H
#define __MY_TOP_WIDGET_H

#include <kxmlguiwindow.h>

class PlayerInfo;
class MyMainView;

class MyTopLevelWidget:public KXmlGuiWindow
{
   Q_OBJECT
public:
   MyTopLevelWidget();
   void start();

private slots:
   void setStatusText(const QString & text,int id);
   void keySetup();
   void energy(int pn,int en);
   void hitPoints(int pn,int hp);
   void wins(int pn,int w);

protected:
   void initActions( );
   void initStatusBar( );
   void initGameWidgets();

private:
   PlayerInfo *playerinfo[2];
   MyMainView *playfield;
};

#endif
