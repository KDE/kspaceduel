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

#include "mathroutines.h"

#include "math.h"

double rectToAngle(double x,double y)
{
   double phi=0;
   if(fabs(x)<1e-6)
   {
      if(y>0)
         phi=M_PI_2;
      else
         phi=-M_PI_2;
   }
   else
   {
      phi=atan(y/x);
      if(x<0)
         phi+=M_PI;
   }
   if(phi>M_PI)
      phi-=2*M_PI;
   return phi;
}

double average(double phi1,double phi2)
{
   return phi2+difference(phi1,phi2)/2.0;
}

double difference(double phi1,double phi2)
{
   double dif;

   dif=phi1-phi2;
   while(dif>M_PI)
      dif-=2*M_PI;
   while(dif<-M_PI)
      dif+=2*M_PI;

   return dif;
}
