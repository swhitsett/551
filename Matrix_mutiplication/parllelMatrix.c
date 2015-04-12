#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reciveMatrix(int n,int matrix[]);
void multiMatrixIJK(int n, int vectorA[n], int matrixB[], int tempVector[]);
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

void multiMatrixIJK(int n, int vectorA[1], int matrixB[], int tempVector[]){
	
	int j,k;
	// int *localVec = NULL;
	// localVec = malloc(n*sizeof(int));

	for(j=0; j<n; j++){
		tempVector[j] = 0;
		for(k=0; k<n; k++){

			tempVector[j] += vectorA[k] * matrixB[k*n+j];
		}
		// tempVector[i];
	}

	int i;
	for(i=0; i<n; i++)
		printf("%d\n",tempVector[i] );
	// return tempVector;
	printf("\n" );
}

void printMatrix(int n, int matrix[]){

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
		int i;
		for(i=0; i<n; i++){
			multiMatrixIJK(n, matrix1, matrix2, tempVector);
			// reslutMatrix[i] = multiMatrixIJK(n, matrix1[i], matrix2, tempVector);
			
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
	free(tempVector);
	free(reslutMatrix);
	return 0;
}