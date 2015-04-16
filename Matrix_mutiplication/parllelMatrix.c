#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>

void reciveMatrix(int n,int matrix[]);
void multiMatrixIJK(int n, int matrixA[], int matrixB[], int reslutC[]);
void printMatrix(int n, int matrix[]);
/*-------------------------------------------------------------------*/
void reciveMatrix(int n, int matrix[] ){
    int row;
    int colum;
    for(row=0; row<n; row++){
        for(colum=0; colum<n; colum++){
            scanf("%i", &matrix[row*n+colum]);
        }
    }
}
/*-------------------------------------------------------------------*/
void multiMatrixIJK(int n, int matrixA[], int matrixB[], int reslutC[]){

}
/*-------------------------------------------------------------------*/
void createMatrix(int n, int matrixA[], int matrixB[]){
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
    int *reslutMatrix = NULL;
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
        reslutMatrix = malloc(n*n*sizeof(int));
        tempVector = malloc(n*sizeof(int));
    

    if(flag == 'R'){
        // createMatrix(n, matrix1, matrix2);
        int row;
        int colum;
        int counter;
        int temp =0;
        srand(time(NULL));

        for(counter=0; counter<2; counter++){
            for(row=0; row<n; row++){
                for(colum=0; colum<n; colum++){

                    temp = (rand()%10);
                    if(counter == 0)
                        matrix1[row*n+colum] = temp;
                    else
                        matrix2[row*n+colum] = temp;
                }
            }
        }
    }
    else if(flag == 'I'){
        reciveMatrix(n, matrix1);
        reciveMatrix(n, matrix2);
    }

    MPI_Barrier(comm);
    start = MPI_Wtime();
   
    // Code to time Goes hereeeeeeeeeeeeeee and what im dividing up !!!!!!
     MPI_Bcast(matrix1, (n * n), MPI_INT, 0, MPI_COMM_WORLD);
     // MPI_Scatter(matrix2, (n*n)/2, MPI_INT, reslutMatrix, (n*n)/2, MPI_INT, 0, MPI_COMM_WORLD);
    
      printf("Greetings from process %d of %d!\n", my_rank, comm_sz);
      printMatrix(n, matrix1);
    
                

   // printMatrix(n, matrix2);   
    finish = MPI_Wtime();
    loc_elapsed = finish - start;
    MPI_Reduce(&loc_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, comm);

    if (my_rank == 0)
        printf("Elapsed time = %f\n", elapsed);

    free(matrix1);
    free(matrix2);
    free(reslutMatrix);
    free(tempVector);
    MPI_Finalize();
    return 0;

    //--------------------------------------------------------------------------- 
}
