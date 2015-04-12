#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reciveMatrix(int n,int matrix[]);
void multiMatrixIJK(int n, int matrixA[], int matrixB[], int reslutC[]);

void reciveMatrix(int n, int matrix[] ){
	int row;
	int colum;
	for(row=0; row<n; row++){
		for(colum=0; colum<n; colum++){
			scanf("%i", &matrix[row*n+colum]);
		}
	}
}

void multiMatrixIJK(int n, int matrixA[], int matrixB[], int reslutC[]){
	
}

int main(){

	char flag;
	char form[3];
	int *matrix1;
	int *matrix2;
	int *reslutMatrix;
	int n;

	scanf("%s %c %d",form, &flag, &n);

	matrix1 = malloc(n*n*sizeof(int));
	matrix2 = malloc(n*n*sizeof(int));
	reslutMatrix = malloc(n*n*sizeof(int));

	reciveMatrix(n, matrix1);
	reciveMatrix(n, matrix2);

	if(strcmp("ijk",form) == 0){
		int i,j,k;
		for(i=0; i<n; i++){
			for(j=0; j<n; j++){
				for(k=0; k<n; k++){
					reslutMatrix[]
				}
			}
		}
	}
	else if(strcmp("ikj",form) == 0)){
		// printf("needs work");// multiMatrixIKJ();
	}
	else if(strcmp("kij",form) == 0)){
		printf("needs work2");// multiMatrixKIJ();
	}
	else
		printf("Please specifiy a correct Form");

	//for printing pourpses --------------
	// int i;
	// int j;
	// for (i = 0; i < n; i++) {
	//      for (j = 0; j < n; j++)
	//         printf("%d ", matrix2[i*n+j]);
	//      printf("\n");
	//   	}

	free(matrix1);
	free(matrix2);
	free(reslutMatrix);
	return 0;
}