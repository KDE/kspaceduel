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
