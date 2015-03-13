//Sam Whitsett

#include <stdio.h>
#include <math.h>

double f(double x);    
double Trap(double a, double b, unsigned long int n, double h);
double realtive_error;

int main(void) {
double tResult;   
double a = 100;
double b = 600;       
unsigned long int n = 3000000; 
double  h;          
int p;

h = (b-a)/n;
tResult = Trap(a, b, n, h);

for(p=1; p<n;p=p*2){
 if(p >= 1000000)
    p = (p / 2) + 100000;

 h = (b-a)/p;
 tResult = Trap(a, b, p, h);
 if(error(tResult) == 1){
    break;
 }
}

printf("With n = %i trapezoids, our estimate\n", p);
printf("of the tResult from %f to %f = %.15f\n",
  a, b, tResult);
printf("%.15g\n", realtive_error );

return 0;
}

//------------------------------------------------------------------
double Trap(double a, double b, unsigned long int n, double h) {
   double tResult;
   int k;

   tResult = (f(a) + f(b))/2.0;
   for (k = 1; k <= n-1; k++) {
     tResult += f(a+k*h);
     // printf("hello");
   }
   tResult = tResult*h;

   return tResult;
} 

//------------------------------------------------------------------
double f(double x) {
   return cos(x/3.0) - 2 * cos(x/5.0) + 5 * sin(x/4.0) + 8;
}

//-----------------------------------------------------------------------
int error(double possible){

   double trueVal = 4003.7209001513;
   double error = 5 * pow(10,(-15));

   double trueErr = trueVal - possible;
 realtive_error = trueErr / trueVal;
   realtive_error = fabs(realtive_error);

   if(realtive_error <= error)
   {
      // printf("%f\n", realtive_error);
      return 1;
   }
   else
   {
      // printf("yayaya");
      return 0;
   }
}
