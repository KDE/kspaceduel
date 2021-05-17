/*
    SPDX-FileCopyrightText: 1998-2001 Andreas Zehender <az@azweb.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "spritebase.h"

#include <math.h>

#include <QGraphicsScene>
#include <QSvgRenderer>


SimpleSprite::SimpleSprite(QSvgRenderer* svg, const QString& element)
      :QGraphicsSvgItem(nullptr)
{
   setSharedRenderer(svg);
   setElementId(element);
   init();
}

void SimpleSprite::init()
{
   m_width = boundingRect().width();
   m_height = boundingRect().height();
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

MobileSprite::MobileSprite(QSvgRenderer* svg, const QString& element, int pn)
      :SimpleSprite(svg, element)
{
   stopped=false;
   playerNumber=pn;
}

void MobileSprite::forward(double mult, int fr)
{
   Q_UNUSED(fr);

   if(!stopped)
   {
      QGraphicsSvgItem::moveBy(xVelocity()*mult,yVelocity()*mult);
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
      QGraphicsSvgItem::moveBy(xVelocity()*mult,yVelocity()*mult);
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

AnimatedSprite::AnimatedSprite(QSvgRenderer* svg,
                               const QList<QString> &animation,
                               int pn)
    : MobileSprite(svg,animation[0],pn)
{
    frames = animation;
    currentFrame = 0;
}

void AnimatedSprite::setFrame(int frame)
{
    if (!frames.isEmpty()) {
        currentFrame = frame % frames.size();
        setElementId(frames.at(currentFrame));
    }
}

void AnimatedSprite::advance(int phase)
{
    if (phase == 1 && !frames.isEmpty()) {
        currentFrame = (currentFrame + 1) % frames.size();
        setElementId(frames.at(currentFrame));
        init();
    }
}

QPainterPath AnimatedSprite::shape() const
{
    QPainterPath path;
    path.addRect(0, 0,
                 boundingRect().width(),
                 boundingRect().height());
    return path;
}

void AnimatedSprite::setAnimation(const QList<QString> &animation)
{
    frames = animation;
    setFrame(0);
}
