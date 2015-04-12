#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reciveMatrix(int n,int matrix[]);
void multiMatrixIJK(int n, int vectorA[], int matrixB[]);
void printMatrix(int n, int matrix[]);

void reciveMatrix(int n, int matrix[] ){
	int row;
	int colum;
	for(row=0; row<n; row++){
		for(colum=0; colum<n; colum++){
			scanf("%i", &matrix[row*n+colum]);
		}
	}
}

void multiMatrixIJK(int n, int vectorA[], int matrixB[]){
	
}
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
int main(){

	char flag;
	char form[3];
	int *matrix1 = NULL;
	int *matrix2 = NULL;
	int *reslutMatrix = NULL;
	int *tempVector = NULL;
	int n;

	scanf("%s %c %d",form, &flag, &n);

	matrix1 = malloc(n*n*sizeof(int));
	matrix2 = malloc(n*n*sizeof(int));
	reslutMatrix = malloc(n*n*sizeof(int));
	tempVector = malloc(n*sizeof(int));

	reciveMatrix(n, matrix1);
	reciveMatrix(n, matrix2);

	if(strcmp("ijk",form) == 0){
		int i,j,k;
		for(i=0; i<n; i++){
			
			empVector[i] = multiMatrixIJK(n, matrix1[i], matrix2);
			
		}
	}
	else if(strcmp("ikj",form) == 0){
		int i,j,k;
	}
	else if(strcmp("kji",form) == 0){
		int i,j,k;
	}
	else
		printf("Please specifiy a correct Form");

	printMatrix(n, reslutMatrix);

	free(matrix1);
	free(matrix2);
	free(reslutMatrix);
	return 0;
}