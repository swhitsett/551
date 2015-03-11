/* File:    trap.c
 * Purpose: Calculate definite integral using trapezoidal 
 *          rule.
 *
 * Input:   a, b, n
 * Output:  Estimate of integral from a to b of f(x)
 *          using n trapezoids.
 *
 * Compile: gcc -g -Wall -o trap trap.c
 * Usage:   ./trap
 *
 * Note:    The function f(x) is hardwired.
 *
 * IPP:     Section 3.2.1 (pp. 94 and ff.) and 5.2 (p. 216)
 */

#include <stdio.h>
#include <math.h>

double f(double x);    /* Function we're integrating */
double Trap(double a, double b, unsigned long int n, double h);
double realtive_error;

int main(void) {
   double  integral;   /* Store result in integral   */
   double  a = 100;
   double  b = 600;       /* Left and right endpoints   */
   unsigned long int     n = 3000000;          /* Number of trapezoids       */
   double  h;          /* Height of trapezoids       */
   int p;

   // printf("Enter a, b, and n\n");
   // scanf("%lf", &a);
   // scanf("%lf", &b);
   // scanf("%lu", &n);
   // int asdf = error(integral);

   // n = n*2;
   h = (b-a)/n;
   integral = Trap(a, b, n, h);

   // while(asdf == 0){
      // n = n*2;
   //4003.720901
      for(p=1; p<n;p++){
         if(p >= 2)
            p = p + 150000;

         h = (b-a)/p;
         integral = Trap(a, b, p, h);
         if(error(integral) == 1){
            break;
         }
         // printf("%f and the n =%i\n", integral,p);
      }
   // }
   
   printf("With n = %i trapezoids, our estimate\n", p);
   printf("of the integral from %f to %f = %.15f\n",
      a, b, integral);
   printf("%.15g\n", realtive_error );

   return 0;
}  /* main */

/*------------------------------------------------------------------
 * Function:    Trap
 * Purpose:     Estimate integral from a to b of f using trap rule and
 *              n trapezoids
 * Input args:  a, b, n, h
 * Return val:  Estimate of the integral 
 */
double Trap(double a, double b, unsigned long int n, double h) {
   double integral;
   int k;

   integral = (f(a) + f(b))/2.0;
   for (k = 1; k <= n-1; k++) {
     integral += f(a+k*h);
     // printf("hello");
   }
   integral = integral*h;

   return integral;
}  /* Trap */

/*------------------------------------------------------------------
 * Function:    f
 * Purpose:     Compute value of function to be integrated
 * Input args:  x
 */
double f(double x) {
   return cos(x/3.0) - 2 * cos(x/5.0) + 5 * sin(x/4.0) + 8;
}  /* f */
//-----------------------------------------------------------------------
int error(double approx){

   double true_value = 4003.7209001513;
   double err = 5 * pow(10,(-15));

   double true_error = true_value - approx;
 realtive_error = true_error / true_value;
   realtive_error = fabs(realtive_error);

   if(realtive_error <= err)
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