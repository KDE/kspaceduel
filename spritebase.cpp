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

#include <math.h>

#include <QGraphicsScene>

#include <kdebug.h>

#include "spritebase.h"

SimpleSprite::SimpleSprite(QPixmap* pixmap, QGraphicsScene* scene)
      :QGraphicsPixmapItem(*pixmap, 0, scene)
{
   init();
}

SimpleSprite::SimpleSprite(QGraphicsItem * parent, QGraphicsScene * scene)
      :QGraphicsPixmapItem(parent, scene)
{
   init();
}

void SimpleSprite::init()
{
   m_width = pixmap().width();
   m_height = pixmap().height();
   m_center = QPointF(m_width/2.0f,m_height/2.0f);
}

int SimpleSprite::width()
{
   return m_width;
}

int SimpleSprite::height()
{
   return m_height;
}

QPointF SimpleSprite::center()
{
   return m_center;
}

MobileSprite::MobileSprite(QPixmap* pixmap, QGraphicsScene* scene, int pn)
      :SimpleSprite(pixmap, scene)
{
   stopped=false;
   playerNumber=pn;
}

MobileSprite::MobileSprite(QGraphicsItem * parent, QGraphicsScene * scene, int pn)
      :SimpleSprite(parent, scene)
{
   stopped=false;
   playerNumber=pn;
}

void MobileSprite::forward(double mult, int fr)
{
   if(!stopped)
   {
      QGraphicsPixmapItem::moveBy(xVelocity()*mult,yVelocity()*mult);
      checkBounds();
      // FIXME
      //setFrame(fr);
   }
  // else
    //  setFrame(fr);
}

void MobileSprite::forward(double mult)
{
   if(!stopped)
   {
      QGraphicsPixmapItem::moveBy(xVelocity()*mult,yVelocity()*mult);
      checkBounds();
   }
}

void MobileSprite::checkBounds()
{
   double cx, cy;
   int ch, cw;

   cx = x();
   cy = y();
   ch = (int)scene()->height();
   cw = (int)scene()->width();

   if ( (int)(cx+0.5) < 0 )
      cx = cw - 1 - fmod( -cx, cw );
   else if ( (int)(cx+0.5) > ( cw-1 ) )
      cx = fmod( cx-cw-1, cw );
   if ( (int)(cy+0.5) < 0 )
      cy = ch-1 - fmod( -cy, ch );
   else if ( (int)(cy+0.5) > ( ch-1 ) )
      cy = fmod( cy-ch-1, ch );
   if ( (cx != x()) || (cy != y()) )
   {
      // printf("%5.2f %5.2f %5.2f %5.2f\n", x(), y(), cx, cy);
      setPos(QPointF(cx, cy));
   }
}

void MobileSprite::calculateGravity(double gravity,double mult)
{
   double abs_2,nx,ny,ex,ey,sq,eg;

   if(!stopped)
   {
      ex=x()-scene()->width()/2.0;
      ey=y()-scene()->height()/2.0;

      abs_2=ex*ex+ey*ey;
      sq=sqrt(abs_2);

      nx=ex/sq;
      ny=ey/sq;
      eg=gravity*mult;
      setVelocity(xVelocity()-eg*nx/abs_2,
                  yVelocity()-eg*ny/abs_2);
   }
}

int MobileSprite::spriteFieldWidth()
{
	return (int)scene()->width();
}

int MobileSprite::spriteFieldHeight()
{
	return (int)scene()->height();
}

void MobileSprite::setVelocity(double vx, double vy)
{
	xvel = vx;
	yvel = vy;
}

AiSprite MobileSprite::toAiSprite()
{
       // y direction: screen:       bottom to top
       //              mathematical: top to bottom
   AiSprite as;
   as.x=x()-scene()->width()/2.0;
   as.y=-(y()-scene()->height()/2.0);
   as.dx=xVelocity();
   as.dy=-yVelocity();
   as.sun=false;
   as.border=false;
   return as;
}

AnimatedSprite::AnimatedSprite(const QList<QPixmap> &animation,
                                        QGraphicsScene *scene, int pn)
    : MobileSprite(0, scene)
{
	frames = animation;
	currentFrame = 0;
	setFrame(0);
}

void AnimatedSprite::setFrame(int frame)
{
    if (!frames.isEmpty()) {
        currentFrame = frame % frames.size();
        setPixmap(frames.at(currentFrame));
    }
}

void AnimatedSprite::advance(int phase)
{
    if (phase == 1 && !frames.isEmpty()) {
        currentFrame = (currentFrame + 1) % frames.size();
        setPixmap(frames.at(currentFrame));
    }
}

QPainterPath AnimatedSprite::shape() const
{
    QPainterPath path;
    path.addRect(0, 0,
                 frames.at(currentFrame).width(),
                 frames.at(currentFrame).height());
    return path;
}

void AnimatedSprite::setAnimation(const QList<QPixmap> &animation)
{
	frames = animation;
	setFrame(0);
}
