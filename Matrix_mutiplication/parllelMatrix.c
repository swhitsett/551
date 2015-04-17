#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>

void reciveMatrix(int n, char flag, int matrix[]);
void multiMatrixIJK(int n, int matrixA[], int matrixB[], int resultC[]);
void printMatrix(int n, int matrix[]);
/*-------------------------------------------------------------------*/
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
void multiMatrixIJK(int n, int matrixA[], int matrixB[], int resultC[]){

}
/*-------------------------------------------------------------------*/
void createMatrix(int n, char flag, int matrixA[], int matrixB[], int matrixC){
    int row;
    int colum;
    int temp;
    for(row=0; row<n; row++){
        for(colum=0; colum<n; colum++){
            temp = (rand()%10);
            matrixB[row*n+colum] = temp;
            printf("%i\n", matrixB[row*n+colum]);
        }
    }

    for(row=0; row<n; row++){
        for(colum=0; colum<n; colum++){
            temp = (rand()%10);
            matrixB[row*n+colum] = (int)1;

        }
    }
}
/*-------------------------------------------------------------------*/
void printMatrix(int n, int matrix[]){
    //for printing pourpses --------------
    int i;
    int j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++)
            printf("%d ", matrix[i*n+j]);
        printf("\n");
    }
    // for(i=0; i<n; i++)
    // 	printf("%d\n",matrix[i] );
}
/*-------------------------------------------------------------------*/
int main(){

    char flag;
    char form[3];
    int *matrix1 = NULL;
    int *matrix2 = NULL;
    int *matrix3 = NULL;
    int *splicedMatrix = NULL;
    int *resultMatrix = NULL;
    int *tempVector = NULL;
    int local_n, n;
    int my_rank, comm_sz;
    double start, finish, loc_elapsed, elapsed;

    MPI_Comm comm;

    MPI_Init(NULL, NULL);
    comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &comm_sz);
    MPI_Comm_rank(comm, &my_rank);

    // allocate arrays and populate matrix
        scanf("%s %c %d",form, &flag, &n);
        MPI_Bcast(&form, 3, MPI_INT, 0, comm);
        MPI_Bcast(&flag, 1, MPI_INT, 0, comm); 
        MPI_Bcast(&n, 1, MPI_INT, 0, comm);

        matrix1 = malloc(n*n*sizeof(int));
        matrix2 = malloc(n*n*sizeof(int));
        splicedMatrix = malloc(((n*n)/comm_sz)*sizeof(int));
        matrix3 = malloc(n*n*sizeof(int));
        resultMatrix = malloc(((n*n)/comm_sz)*sizeof(int));
        tempVector = malloc(n*sizeof(int));
    
    if(my_rank == 0){
        reciveMatrix(n, flag, matrix1);
        reciveMatrix(n, flag, matrix2);
        reciveMatrix(n, 'C', matrix3);
    }

    MPI_Barrier(comm);
    start = MPI_Wtime();
   
    // Code to time Goes hereeeeeeeeeeeeeee and what im dividing up !!!!!!
     MPI_Bcast(matrix2, (n * n), MPI_INT, 0, MPI_COMM_WORLD);
  //   MPI_Bcast(resultMatrix, (n*n), MPI_INT, 0, MPI_COMM_WORLD);
     MPI_Scatter(matrix3, (n*n)/comm_sz, MPI_INT, resultMatrix, (n*n)/comm_sz, MPI_INT, 0, MPI_COMM_WORLD);
     MPI_Scatter(matrix1, (n*n)/comm_sz, MPI_INT, splicedMatrix, (n*n)/comm_sz, MPI_INT, 0, MPI_COMM_WORLD);
    
     //  printf("Greetings from process %d of %d!\n", my_rank, comm_sz);
     // // printMatrix(n, splicedMatrix);
     // for(int i=0; i<(n)/comm_sz; i++){
     //     for(int j=0; j<n; j++){
     //        printf("%d,", splicedMatrix[i*n+j]);
     //     }
     //     printf("\n");
     // }
    if(strcmp("ijk",form) == 0){
        int i,j,k;
        for(i=0; i<(n)/comm_sz; i++){
            for(j=0; j<n; j++){
                for(k=0; k<n; k++){

                    resultMatrix[i*n+j] += splicedMatrix[i*n+k] * matrix2[k*n+j];
                }
            }
        }
    }
                
    MPI_Gather(resultMatrix, (n*n)/comm_sz, MPI_INT,
                    matrix3, (n*n)/comm_sz, MPI_INT, 0,
                                    MPI_COMM_WORLD);

   finish = MPI_Wtime();
   loc_elapsed = finish - start;

   if(my_rank == 0){
   printf("Elapsed time = %f\n", loc_elapsed);
   printMatrix(n, matrix3);
   }

    free(matrix1);
    free(matrix2);
    free(splicedMatrix);
    free(tempVector);
    MPI_Finalize();
    return 0;

    //--------------------------------------------------------------------------- 
}
