/*
    Sam Whitsett
    Assignment 3: MPI Matrix Multiplicaion
    04/17/15

    File: parallelMatrix.c

    Compile: mpicc -g -Wall -std=c99 -o matrixM parllelMatrix.c

    Run: matrixM

         Input as follows....
         Form (ie. ijk, ikj, kij)
         Flag (ie. 'I'(manual input) or 'R'(random input 0-9))
         N    (ie. number of rows and colums for the matrix 'N * N')

         If flag 'I' is selected matrix rows will be entered by entering a number
         followed by a space. A new row can be entered by pressing return.
    
    Output: Elapsed time of execution for multiplication

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>

void ijkForm(int n, int comm_sz, int matrixA[], int matrixB[], int resultC[]);
void ikjForm(int n, int comm_sz, int matrixA[], int matrixB[], int resultC[]);
void kijForm(int n, int comm_sz, int matrixA[], int matrixB[], int resultC[]);
void reciveMatrix(int n, char flag, int matrix[]);
void printMatrix(int n, int matrix[]);
/*-------------------------------------------------------------------*/
/*  Function: reciveMatrix
    
    Variables:
        n: index size of the matrix
        flag: 'I,R,C' I = manual input, R = random input, C = zero out
        matrix[]: desired matrix to opperat uppon

    Purpose: Function will loop through the rows and colums to create a 
             matrix by manual input, randomly generated or just to populate
             with 0's to ensure no garbage values     
  -------------------------------------------------------------------*/
void reciveMatrix(int n, char flag, int matrix[] ){
    int row;
    int colum;
    srand(time(NULL));

    for(row=0; row<n; row++){
        for(colum=0; colum<n; colum++){
            if(flag == 'I')
                scanf("%i", &matrix[row*n+colum]);
            else if(flag == 'R')
                matrix[row*n+colum] = (rand()%10);
            else if(flag == 'C')
                matrix[row*n+colum] = 0;
            else
                printf("unspecificed form");
        }
    }
}
/*-------------------------------------------------------------------*/
/*  Function: printMatrix
    
    Variables:
        n: index size of the matrix
        matrix[]: desired matrix to display to std out

    Purpose: Function will loop through the rows and colums to output the
             provided matrix   
  -------------------------------------------------------------------*/
void printMatrix(int n, int matrix[]){
    int i;
    int j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++)
            printf("%d ", matrix[i*n+j]);
        printf("\n");
    }
}
/*-------------------------------------------------------------------*/
/*  Function: ijkForm
    
    Variables:
        n: index size of the matrix
        comm_sz: the amount of processors used to process the calculations
        matrixA[]: A partial matrix from the original matrix1
        matrixB[]: The full matrixB
        resultC[]: A blank matrix with the same demensions as matrixA

    Purpose: Function will calculate the dot product of matrixA x matrixB
             using the ijk form and place the partial result in resultC 
             for MPI_Gather to compile into the final result of matrix3
  -------------------------------------------------------------------*/
void ijkForm(int n, int comm_sz, int matrixA[], int matrixB[], int resultC[]){

    int i,j,k;
    for(i=0; i<(n)/comm_sz; i++){
        for(j=0; j<n; j++){
            for(k=0; k<n; k++){
                resultC[i*n+j] += matrixA[i*n+k] * matrixB[k*n+j];
            }
        }
    }
}
/*-------------------------------------------------------------------*/
/*  Function: ikjForm
    
    Variables:
        n: index size of the matrix
        comm_sz: the amount of processors used to process the calculations
        matrixA[]: A partial matrix from the original matrix1
        matrixB[]: The full matrixB
        resultC[]: A blank matrix with the same demensions as matrixA

    Purpose: Function will calculate the dot product of matrixA x matrixB
             using the ikj form and place the partial result in resultC 
             for MPI_Gather to compile into the final result of matrix3
  -------------------------------------------------------------------*/
void ikjForm(int n, int comm_sz, int matrixA[], int matrixB[], int resultC[]){

    int i,j,k;
    for(i=0; i<(n)/comm_sz; i++){
        for(k=0; k<n; k++){
            for(j=0; j<n; j++){
                resultC[i*n+j] += matrixA[i*n+k] * matrixB[k*n+j];
            }
        }
    }
}
/*-------------------------------------------------------------------*/
/*  Function: kijForm
    
    Variables:
        n: index size of the matrix
        comm_sz: the amount of processors used to process the calculations
        matrixA[]: A partial matrix from the original matrix1
        matrixB[]: The full matrixB
        resultC[]: A blank matrix with the same demensions as matrixA

    Purpose: Function will calculate the dot product of matrixA x matrixB
             using the kij form and place the partial result in resultC 
             for MPI_Gather to compile into the final result of matrix3
  -------------------------------------------------------------------*/
void kijForm(int n, int comm_sz, int matrixA[], int matrixB[], int resultC[]){

    int i,j,k;
    for(k=0; k<n; k++){
        for(i=0; i<n/comm_sz; i++){
            for(j=0; j<n; j++){
                resultC[i*n+j] += matrixA[i*n+k] * matrixB[k*n+j];
            }
        }
    }
}
/*-------------------------------------------------------------------*/
/*  The Main function will............
    -Declare Varibles for processing
    -Recive input for the form, flag, and size(n)
    -Set up communications for MPI
    -Brodcast varibles using MPI
    -Allocate memory for all Matrices
    -Scatter smaller parts of desired Matrices
    -Call the Mutiplication function set by 'form'
    -Take the complete time and display it to the user with the help
     of MPI

  -------------------------------------------------------------------*/
int main(){

    //Declare Varibles
    char flag;
    char form[3];
    int *matrix1 = NULL;
    int *matrix2 = NULL;
    int *matrix3 = NULL;
    int *splicedMatrix = NULL;
    int *resultMatrix = NULL;
    int spliced_sz, n;
    int my_rank, comm_sz;
    double start, finish, loc_elapsed;

    //Set MPI communications
    MPI_Comm comm;
    MPI_Init(NULL, NULL);
    comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &comm_sz);
    MPI_Comm_rank(comm, &my_rank);

    //Recive user Input
    scanf("%s %c %d",form, &flag, &n);

    //Brodcast user input
    MPI_Bcast(&form, 3, MPI_INT, 0, comm);
    MPI_Bcast(&flag, 1, MPI_INT, 0, comm); 
    MPI_Bcast(&n, 1, MPI_INT, 0, comm);
    spliced_sz = (n*n)/comm_sz;

    //Allocate memory
    matrix1 = malloc(n * n * sizeof(int));
    matrix2 = malloc(n * n * sizeof(int));
    matrix3 = malloc(n * n * sizeof(int));
    splicedMatrix = malloc(spliced_sz * sizeof(int));
    resultMatrix = malloc(spliced_sz * sizeof(int));

    //Populate Matrices if it is the parent process
    if(my_rank == 0){
        reciveMatrix(n, flag, matrix1);
        reciveMatrix(n, flag, matrix2);
        reciveMatrix(n, 'C', matrix3);
    }

    //start time
    MPI_Barrier(comm);
    start = MPI_Wtime();
    //-------------------------------------------To Be Timed-------------------------------------------
    //Distribution of matrices using scatter and brodcast
    MPI_Bcast(matrix2, (n * n), MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(matrix1, spliced_sz, MPI_INT, splicedMatrix, spliced_sz, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(matrix3, spliced_sz, MPI_INT, resultMatrix, spliced_sz, MPI_INT, 0, MPI_COMM_WORLD);

    //Run the functions set by form
    if(strcmp("ijk",form) == 0)
        ijkForm(n, comm_sz, splicedMatrix, matrix2, resultMatrix);
    else if(strcmp("ikj",form) == 0)
        ikjForm(n, comm_sz, splicedMatrix, matrix2, resultMatrix);
    else if(strcmp("kij",form) == 0)
        kijForm(n, comm_sz, splicedMatrix, matrix2, resultMatrix);

    //Gather partial matrices back into matrix3
    MPI_Gather(resultMatrix, (n*n)/comm_sz, MPI_INT,
            matrix3, (n*n)/comm_sz, MPI_INT, 0,
            MPI_COMM_WORLD);
    //---------------------------------------------------------------------------------------------------
    //End time
    finish = MPI_Wtime();
    loc_elapsed = finish - start;

    //Output Results
    if(my_rank == 0){
        printf("running on %d processors\n", comm_sz);
        printf("Elapsed time = %f seconds\n", loc_elapsed);

        if(flag == 'I')
            printMatrix(n, matrix3);
    }

    //Free memory even though bash will do it for you.... i think
    free(matrix1);
    free(matrix2);
    free(matrix3);
    free(splicedMatrix);
    MPI_Finalize();
    return 0;

}
