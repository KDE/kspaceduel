#include "duelwidget.h"
#include <qlayout.h>

DuelWidget::DuelWidget(QWidget *parent,const char *name)
      :QWidget(parent,name)
{
   playerinfo[0]=new PlayerInfo(0,this);
   playerinfo[1]=new PlayerInfo(1,this);
   playfield=new MyMainView(this);
   
   QBoxLayout *toplayout=new QHBoxLayout(this);
   toplayout->addWidget(playerinfo[0]);
   toplayout->addWidget(playfield);
   toplayout->addWidget(playerinfo[1]);
   toplayout->activate();

   playfield->setFocusPolicy(QWidget::StrongFocus);
   playfield->setFocus();

   QObject::connect(playfield,SIGNAL(energy(int,int)),
                    SLOT(energy(int,int)));
   QObject::connect(playfield,SIGNAL(hitPoints(int,int)),
                    SLOT(hitPoints(int,int)));
   QObject::connect(playfield,SIGNAL(wins(int,int)),SLOT(wins(int,int)));
   QObject::connect(playfield,SIGNAL(setStatusText(const char*,int)),
                    SIGNAL(setStatusText(const char*,int)));
}

void DuelWidget::newGame()
{
   playfield->newGame();
}

void DuelWidget::newRound()
{
   playfield->newRound();
}

void DuelWidget::keySetup()
{
   playfield->keySetup();
}

void DuelWidget::gameSetup()
{
   playfield->gameSetup();
}

void DuelWidget::aiSetup()
{
   playfield->aiSetup();
}

void DuelWidget::hitpointSetup()
{
   playfield->hitpointSetup();
}

void DuelWidget::graphicSetup()
{
   playfield->graphicSetup();
}

void DuelWidget::writeConfig()
{
   playfield->writeConfig();
}

void DuelWidget::energy(int pn,int en)
{
   playerinfo[pn]->setEnergy(en);
}

void DuelWidget::hitPoints(int pn,int hp)
{
   playerinfo[pn]->setHitpoints(hp);
}

void DuelWidget::wins(int pn,int w)
{
   playerinfo[pn]->setWins(w);
}

void DuelWidget::readConfig(KConfig *cfg)
{
   playfield->readConfig(cfg);
}
