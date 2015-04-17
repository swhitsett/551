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
/*-------------------------------------------------------------------*/
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

int main(){

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
    spliced_sz = (n*n)/comm_sz;

    matrix1 = malloc(n * n * sizeof(int));
    matrix2 = malloc(n * n * sizeof(int));
    matrix3 = malloc(n * n * sizeof(int));
    splicedMatrix = malloc(spliced_sz * sizeof(int));
    resultMatrix = malloc(spliced_sz * sizeof(int));

    if(my_rank == 0){
        reciveMatrix(n, flag, matrix1);
        reciveMatrix(n, flag, matrix2);
        reciveMatrix(n, 'C', matrix3);
    }

    MPI_Barrier(comm);
    start = MPI_Wtime();
    //---------------------------------------------------------------------------------------------------
    MPI_Bcast(matrix2, (n * n), MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(matrix1, spliced_sz, MPI_INT, splicedMatrix, spliced_sz, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(matrix3, spliced_sz, MPI_INT, resultMatrix, spliced_sz, MPI_INT, 0, MPI_COMM_WORLD);

    if(strcmp("ijk",form) == 0)
        ijkForm(n, comm_sz, splicedMatrix, matrix2, resultMatrix);
    else if(strcmp("ikj",form) == 0)
        ikjForm(n, comm_sz, splicedMatrix, matrix2, resultMatrix);
    else if(strcmp("kij",form) == 0)
        kijForm(n, comm_sz, splicedMatrix, matrix2, resultMatrix);

    MPI_Gather(resultMatrix, (n*n)/comm_sz, MPI_INT,
            matrix3, (n*n)/comm_sz, MPI_INT, 0,
            MPI_COMM_WORLD);
    //---------------------------------------------------------------------------------------------------
    finish = MPI_Wtime();
    loc_elapsed = finish - start;

    if(my_rank == 0){
        printf("Elapsed time = %f\n", loc_elapsed);
        printMatrix(n, matrix3);
    }

    free(matrix1);
    free(matrix2);
    free(matrix3);
    free(splicedMatrix);
    MPI_Finalize();
    return 0;

    //--------------------------------------------------------------------------- 
}
