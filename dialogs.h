/*
    Copyright (C) 1998-2001 Andreas Zehender <az@azweb.de>

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

#ifndef KSPACEDUEL_DIALOGS_H
#define KSPACEDUEL_DIALOGS_H

class QComboBox;
class QTabWidget;
class QSlider;
class QLCDNumber;

#include <KConfigDialog>

#include "defines.h"
#include "structs.h"


class ConfigSetup:public QWidget
{
   Q_OBJECT
public:
   explicit ConfigSetup(SConfig *custom,QWidget *parent = nullptr);

   bool hasChanged();
   bool isDefault();
   void updateSettings();
   void updateWidgets();
   void updateWidgetsDefault();

signals:
   void changed();

protected slots:
   void configSelected(int num);
   void sliderChanged(int val);
protected:   
   void valueChanged(int ednum,int val);
   void valueChanged(int ednum,double val);
   void displayConfig(const SConfig &cfg);
   void setValue(int ednum,int val);
   void setValue(int ednum,double val);
   void setValue(int ednum,unsigned val);
private:
   enum {EditGamespeed=0,
         EditShotSpeed,EditShotEnergyNeed,EditMaxBullets,EditBulletDamage,
         EditBulletLifeTime,EditBulletReloadTime,
         EditMineFuel,EditMineEnergyNeed,EditMineActivateTime,
         EditMineDamage,EditMaxMines,EditMineReloadTime,
         EditAcc,EditEnergyNeed,EditRotationSpeed,
         EditRotationEnergyNeed,EditShipDamage,
         EditSunEnergy,EditGravity,
         EditPosX,EditPosY,EditVelX,EditVelY,
         EditPowerupRefreshTime,EditPowerupLifeTime,
         EditPowerupEnergyAmount,EditPowerupShieldAmount,
         EditNum};
   enum {TabGeneral=0,TabBullet,TabMine,
         TabShip,TabSun,TabStart,TabPowerups,TabNum};
   enum Type {VarInt,VarFloat};
   
   static char LabelName[EditNum][25];
   static int Parent[EditNum];
   static int Position[EditNum];
   static const char *TabName[TabNum];
   static double EditVal[EditNum][3];
   static int EditDiv[EditNum];
   static Type VarType[EditNum];
 
   QTabWidget *tabs;

   QSlider *slider[EditNum];
   QLCDNumber *value[EditNum];
   
   QComboBox *configCombo;
   SConfig *customConfig,config;
   
   int selectedConfig;
};

class SettingsDialog : public KConfigDialog
{
        Q_OBJECT
public:
        explicit SettingsDialog(SConfig *customConfig, QWidget *parent = nullptr, const char *name = nullptr);
        ~SettingsDialog();

signals:
        void settingsUpdated();

private slots:
        void updateWidgets() Q_DECL_OVERRIDE;
        void updateWidgetsDefault() Q_DECL_OVERRIDE;
        void updateSettings() Q_DECL_OVERRIDE;
                                        
private:
        bool hasChanged() Q_DECL_OVERRIDE;
        bool isDefault() Q_DECL_OVERRIDE;

private:
        ConfigSetup* cs;
};

#endif // KSPACEDUEL_DIALOGS_H
