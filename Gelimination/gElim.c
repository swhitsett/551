/*

 * CSCI Assingnment 4 Gaussian Elimination
 *
 *
 *  To compile: gcc -fopenmp -lm -Wall -o gElim gElim.c
 *
 *  To run: ./gElim matrix_size desired_threads
 *
 *  Input:
 *      matrix_size : desired size of the n x n matrix
 *      desired_threads : desired threads to be used in parallized sections
 *
 *  Output will consist of: the amount of processors, the amout of threads used,
 *      over time of execution and the l^2 norm.
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <omp.h>

int threads;            //Global Varible for Threads specified by user
int threads_used;       //Global Varible Threads actually used by program

/*-------------------------------------------------------------------*/
/*  Function: createMatrix
    
    Variables:
        A: The desired matrix n x n
        B: The result vector n
        initMatrix: A 2nd copy of the Matrix n x n (same values)
        n: The desired size of the matrix

    Purpose: Function will loop through the rows and colums to create a 
             matrix by randomly generated input using drand48()     
  -------------------------------------------------------------------*/
void createMatrix(double** A, double* B, 
                  double** initMatrix,
                  double* initVec, int n){
    int i;
    int j;
    double tmp;
    srand48(time(NULL));

    for(i=0; i<n; i++){
        A[i] = malloc(n * sizeof(double));
        initMatrix[i] = malloc(n * sizeof(double));

        for(j=0; j<(n+1); j++){
            tmp = drand48() * 2e6 - 1e6;
            A[i][j] = tmp;
            initMatrix[i][j] = tmp;
        }
        tmp = drand48() * 2e6 - 1e6;
        B[i] = tmp;
        initVec[i] = tmp;
    }
}
/*-------------------------------------------------------------------*/
/*  Function: upperTriangular
    
    Variables:
        A: Populated n x n matrix
        B: Populated vector of n size
        n: max index size

    Purpose: contains the following fuctions of Gaussian Elimination
             partial pivioting - relocation of rows with larger values
                                 before the matrix is converted to 
                                 upper triangular form.
             upper Triangular form  - Conversion of the n x n matrix
                                      to upper triangular form using
                                      annilation.
  -------------------------------------------------------------------*/
void upperTriangular(double **A, double* B, int n){
    int i;
    int j;
    int k;
    int greatest;
    double tmp;
    double tmp2;
    double maxVal;
    
    for(i = 0; i < n-1;i++){
        
        greatest = i;

        #pragma omp parallel for num_threads(threads) default(none) \
                       shared(n, A, B, i, greatest, threads_used) private(maxVal, j)
        for(j=i; j<n; j++){
            if(i == 0)
               threads_used = omp_get_num_threads();

            if(fabs(A[j][i]) > fabs(maxVal)){
                maxVal = A[j][i];
                greatest = j;
            }
        }

        for(k=i; k<n; k++){
            tmp = A[greatest][k];
            A[greatest][k] = A[i][k];
            A[i][k] = tmp;
        }
        tmp2 = B[i];
        B[i] = B[greatest];
        B[greatest] = tmp2;

        #pragma omp parallel for num_threads(threads) default(none) \
                shared(A, B, i, n) private(tmp, j, k)
        for(j=(i+1); j<n; j++){ 
            tmp = A[j][i] / A[i][i];
            for(k=i; k<n; k++){
                A[j][k] -= tmp * A[i][k];
            }
            B[j] -= tmp * B[i];
        }
    }
}
/*-------------------------------------------------------------------*/
/*  Function: backSub
    
    Variables:
        A: Populated n x n matrix in upper triangular form
        B: Populated vector of n size
        C: Blank matrix to hold values calculated of B[]/A[][]
        n: max index size

    Purpose: Performs bac subistution on the matrix A which is
             currently in upper triangular form.
  -------------------------------------------------------------------*/
void backSub(double** A, double* B, double* C, int n){

    int i;
    int j;
    for(i=(n-1); i>=0; i--){
        C[i] = B[i] / A[i][i];
        #pragma omp parallel for num_threads(threads) default(none) \
                shared(A, B, n, i, C) private(j)
        for(j=(n-1); j>=0; j--){
            B[j] -= A[j][i] * C[i];
        }
    }
}
/*-------------------------------------------------------------------*/
/*  Function: calcResidual
    
    Variables:
        initMatrix: untouched n x n matrix 
        initVec: untouched solution array
        C: values calculated from backwards substitution B[]/A[][]
        X: Blank array to hold the values of each rows residual value
        n: index

    Purpose: Calculates the residual, showing the diffrence from the 
             original values to the approxmate
  -------------------------------------------------------------------*/
void calcResidual(double** initMatrix, double* initVec,
                     double* C, double* X, int n){

    int i;
    int j;
    #pragma omp parallel for num_threads(threads) default(none) \
        shared(initMatrix, initVec, C, n, X) private(i, j)
    for(i=0; i<n; i++){
        X[i] = 0.0;
        for(j=0; j<n; j++){
            X[i] += initMatrix[i][j] * C[j];
        }
        X[i] = initVec[i] - X[i];
    }
}
/*-------------------------------------------------------------------*/
/*  Function: l2norm
    
    Variables:
            X: array of size n holding the residual values
            L2: result of the calculated L2 Norm
            n: index
        

    Purpose: Calculates the L^2 norm
  -------------------------------------------------------------------*/
void l2norm(double* X, double* L2, int n){
    
    double result = 0.0;
    int i;
    #pragma omp parallel for num_threads(threads) default(none) \
        shared(result, X, n) private(i)
    for(i=0; i<n; i++){
        result += pow(X[i], 2);
    }
    *L2 = sqrt(result);
}
/*-------------------------------------------------------------------*/
/*  The Main function will............
    -Declare Varibles for processing
    -Recive input for the the matrix size and desired threads
    -Allocate memory for all Matrices and varibles
    -Call the above functions to complete a Gaussian Equation.
    -Take the complete time and display it to the user

  -------------------------------------------------------------------*/
int main(int argc, char* argv[]){
    int n;
    // int i;
    // int j;
    double start, finished, elapsed;
    double** initMatrix;
    double* initVec;
    double** A;
    double* B;
    double* C;
    double* X;
    double* L2;
    
    // ---- Used for MTL values were hard coded -----
    // n = 8000;
    // #pragma omp parallel	
    // threads = omp_get_num_threads(); 
    //-----------------------------------------------

    if(argc == 3) {
       n = atoi(argv[1]);
       threads = atoi(argv[2]);
    }
    else{
       printf("invalid input");
       return 0;
    }
    
    initMatrix = malloc(n * sizeof(double*));
    initVec = malloc(n * sizeof(double));
    A = malloc(n * sizeof(double*));
    B = malloc(n * sizeof(double));
    C = malloc(n * sizeof(double));
    X = malloc(n * sizeof(double));
    L2 = malloc(sizeof(double));


    createMatrix(A, B, initMatrix, initVec, n);
    
    start = omp_get_wtime();

    upperTriangular(A, B, n);
    backSub(A, B, C, n);

    finished = omp_get_wtime();
    elapsed = finished - start;

    calcResidual(initMatrix, initVec, C, X, n);
    l2norm(X, L2, n);

    printf("Processors used: %d\n", omp_get_num_procs());
    printf("Threads used: %d\n", threads_used);
    printf("Elapsed time: %f\n", elapsed);
    printf("L2 norm using residuals: %f\n", *L2);
    return 0;
}
