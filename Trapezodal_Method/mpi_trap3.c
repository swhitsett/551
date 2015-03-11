/* File:     mpi_trap3.c
 * Purpose:  Use MPI to implement a parallel version of the trapezoidal 
 *           rule.  This version uses collective communications to 
 *           distribute the input data and compute the global sum.
 *
 * Input:    The endpoints of the interval of integration and the number
 *           of trapezoids
 * Output:   tResult of the integral from a to b of f(x)
 *           using the trapezoidal rule and n trapezoids.
 *
 * Compile:  mpicc -g -Wall -o mpi_trap2 mpi_trap2.c
 * Run:      mpiexec -n <number of processes> ./mpi_trap2
 *
 * Algorithm:
 *    1.  Each process calculates "its" interval of
 *        integration.
 *    2.  Each process tResults the integral of f(x)
 *        over its interval using the trapezoidal rule.
 *    3a. Each process != 0 sends its integral to 0.
 *    3b. Process 0 sums the calculations received from
 *        the individual processes and prints the result.
 *
 * Note:  f(x) is all hardwired.
 *
 * IPP:   Section 3.4.2 (pp. 104 and ff.)
 */
#include <stdio.h>
#include <mpi.h>
#include <math.h>

/* Get the input values */
void Get_input(int my_rank, int comm_sz, double* a_p, double* b_p, int* n_p);
double Trap(double a, double b, int n, double h);    
double f(double x);
int error(double possible); 

int main(void) {
   int my_rank, comm_sz, local_n;
   int n;   
   double a;
   double b; 
   double h, local_a, local_b;
   double local_int, total_int;

   /* Let the system do what it needs to start up MPI */
   MPI_Init(NULL, NULL);

   /* Get my process rank */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

   /* Find out how many processes are being used */
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

   Get_input(my_rank, comm_sz, &a, &b, &n);
//***************************************************************************
   h = (b-a)/n;          /* h is the same for all processes */
   MPI_Barrier(comm);
   local_start = MPIWtime();

   local_n = n/comm_sz;  /* So is the number of trapezoids  */

   /* Length of each process' interval of
    * integration = local_n*h.  So my interval
    * starts at: */
   local_a = a + my_rank*local_n*h;
   local_b = local_a + local_n*h;
   local_int = Trap(local_a, local_b, local_n, h);

   /* Add up the integrals calculated by each process */
   MPI_Reduce(&local_int, &total_int, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
   
   for(p=1; p<n;p++){
      if(p >= 2)
         p = p * 2;

      h = (b-a)/p;
      tResult = Trap(a, b, p, h);
      if(error(tResult) == 1){
         break;
      }
   }

   /* Print the result */
   if (my_rank == 0) {
      printf("With n = %d trapezoids, our tResult\n", n);
      printf("of the integral from %f to %f = %.15e\n",
          a, b, total_int);
   }
   local_finish = MPI_Wtime();
   local_elapsed = local_finish - local_start;
   MPI_Reduce(&local_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, comm);
   if(my_rank == 0)
      printf("Elapsed time = %e seconds\n", elapsed);
//**************************************************************************
   /* Shut down MPI */
   MPI_Finalize();

   return 0;
} /*  main  */

/*------------------------------------------------------------------
 * Function:     Get_input
 * Purpose:      Get the user input:  the left and right endpoints
 *               and the number of trapezoids
 * Input args:   my_rank:  process rank in MPI_COMM_WORLD
 *               comm_sz:  number of processes in MPI_COMM_WORLD
 * Output args:  a_p:  pointer to left endpoint               
 *               b_p:  pointer to right endpoint               
 *               n_p:  pointer to number of trapezoids
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
}  /* Get_input */

/*------------------------------------------------------------------
 * Function:     Trap
 * Purpose:      Serial function for estimating a definite integral 
 *               using the trapezoidal rule
 * Input args:   a
 *               b
 *               n 
 *               h
 * Return val:   Trapezoidal rule tResult of integral from
 *               a to b using n
 *               trapezoids
 */
double Trap(double a, double b, int n, double h) {
   double tResult, x; 
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
 * Purpose:     Compute value of function to be integrated
 * Input args:  x
 */
double f(double x) {
   return cos(x/3.0) - 2 * cos(x/5.0) + 5 * sin(x/4.0) + 8;
}

//---------------------------------------------------------------
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