/* mpi Trapezoidal Method 
   Sam Whitsett
   03/12/15
*/
#include <stdio.h>
#include <mpi.h>
#include <math.h>

double tResult;
double relative_error;

void Get_input(int my_rank, int comm_sz, double* a_p, double* b_p, int* n_p);
double Trap(double a, double b, int n, double h);    
double f(double x);
int relError(double possible); 

/*------------------------------------------------------------------
 * Function:     Main
   - Contains the action to find the minimum number of trapazoids.
   - Using calls to other sub-functions and in cooperation with MPI functions
     the processes will compute the intergral to find the minimum number of tarapazoids.
     Durring this time the run will be distributed over mutiple processes, where the time
     will also be monitored using MPI
 */
int main(void) {

   int my_rank, comm_sz, local_n;
   int n;
   int p;   
   double a;
   double b;
   double h, local_a, local_b;
   double local_int, total_int;
   double local_start, local_finish, local_elapsed, elapsed;

   MPI_Init(NULL, NULL);
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

   Get_input(my_rank, comm_sz, &a, &b, &n);
   MPI_Barrier(MPI_COMM_WORLD);
   local_start = MPI_Wtime();
//*****************************To be Timed*********************************
   h = (b-a)/n;

   local_n = n/comm_sz;
   local_a = a + my_rank*local_n*h;
   local_b = local_a + local_n*h;
   local_int = Trap(local_a, local_b, local_n, h);

   for(p=1; p<n;p=p*2){

      if(p >= 1000000)
         p = (p / 2) + 100000;

      h = (b-a)/p;
      tResult = Trap(a, b, p, h);
      if(relError(tResult) == 1){
         break;
      }
   }
//**************************************************************************   
   local_finish = MPI_Wtime();
   local_elapsed = local_finish - local_start;
   
   MPI_Reduce(&local_int, &total_int, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
   MPI_Reduce(&local_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

   if (my_rank == 0){
      printf("Running on %i processors.\n", comm_sz);
      printf("Elapsed time = %e seconds\n", elapsed);
      printf("With n = %d trapezoids,\n", p);
      printf("our estimate of the integral from %f to %f = %.15f\n", a, b, total_int);
      printf("absolute relative true error = %e is NOT less than criteria = %e\n"
             , relative_error, 5 * pow(10,(-15))); 
   }
   MPI_Finalize();

   return 0;
} 

/*------------------------------------------------------------------
 * Function:     Get_input
   - This function will grab input for a, b, n to set the bounds for the
     experiment
 */
void Get_input(int my_rank, int comm_sz, double* a_p, double* b_p,
      int* n_p) {

   if (my_rank == 0) {
      printf("Enter a, b, and n\n");
      scanf("%lf %lf %d", a_p, b_p, n_p);
   } 
   MPI_Bcast(a_p, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
   MPI_Bcast(b_p, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
   MPI_Bcast(n_p, 1, MPI_INT, 0, MPI_COMM_WORLD);
}

/*------------------------------------------------------------------
 * Function:     Trap
   - This is the intergration function where bounds are set by user input
   - includes calls to the "f" function
   - returns the total local result
 */
double Trap(double a, double b, int n, double h) {
   // double tResult, x; 
   int i;

   tResult = (f(a) + f(b))/2.0;

   for (i = 1; i <= n-1; i++) {
      tResult += f(a+i*h);
   }
   tResult = tResult*h;

   return tResult;
}

/*------------------------------------------------------------------
 * Function:    f
   - This is the actual equation used to compute the number of Trapezoids
 */
double f(double x) {
   return cos(x/3.0) - 2 * cos(x/5.0) + 5 * sin(x/4.0) + 8;
}

/*---------------------------------------------------------------
   Function:    relError
   - This function computes the absolute relative approximate error that 
     is checked after every series of runs to see if the error is below 
     the desired amount.
*/
int relError(double possible){

   double trueVal = 4003.7209001513;
   double error = 5 * pow(10,(-15));

   relative_error = (trueVal - possible)/ trueVal;
   relative_error = fabs(relative_error);

   if(relative_error <= error)
      return 1;
   else
      return 0;
}
