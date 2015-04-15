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
	int *local_matrix1 = NULL;
	int *local_matrix2 = NULL;
	int *local_reslutMatrix = NULL;
	int *local_tempVector = NULL;
	int local_n, n;
	int my_rank, comm_sz;
	double start, finish, loc_elapsed, elapsed;

	MPI_Comm comm;

	MPI_Init(NULL, NULL);
	comm = MPI_COMM_WORLD;
	MPI_Comm_size(comm, &comm_sz);
	MPI_Comm_rank(comm, &my_rank);

	// allocate arrays and populate matrix
	scanf(" %3s %c %d",&form, flag, &n);

    local_matrix1 = malloc(n*n*sizeof(int));
	local_matrix2 = malloc(n*n*sizeof(int));
	local_reslutMatrix = malloc(n*n*sizeof(int));
	local_tempVector = malloc(n*sizeof(int));

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
						local_matrix1[row*n+colum] = temp;
					else
						local_matrix2[row*n+colum] = temp;
				}
			}
		}
	}
	else if(flag == 'I'){
		reciveMatrix(n, local_matrix1);
		reciveMatrix(n, local_matrix2);
	}

	MPI_Barrier(comm);
   start = MPI_Wtime();

	// Code to time Goes hereeeeeeeeeeeeeee and what im dividing up !!!!!!

   finish = MPI_Wtime();
   loc_elapsed = finish - start;
   MPI_Reduce(&loc_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, comm);

   if (my_rank == 0)
      printf("Elapsed time = %e\n", elapsed);

   free(local_matrix1);
	free(local_matrix2);
	free(local_reslutMatrix);
	free(local_tempVector);
   MPI_Finalize();
   return 0;

//--------------------------------------------------------------------------- 
}
