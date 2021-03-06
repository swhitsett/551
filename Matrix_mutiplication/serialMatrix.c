#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void reciveMatrix(int n,int matrix[]);
void multiMatrixIJK(int n, int matrixA[], int matrixB[], int reslutC[]);
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

void multiMatrixIJK(int n, int matrixA[], int matrixB[], int reslutC[]){
	
}
void createMatrix(char flag, int n, int matrix1[], int matrix2[]){
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

	char *flag;
	char *form[3];
	int *matrix1 = NULL;
	int *matrix2 = NULL;
	int *reslutMatrix = NULL;
	int *tempVector = NULL;
	int n;

	scanf("%s %c %d",*form, flag, &n);

	matrix1 = malloc(n*n*sizeof(int));
	matrix2 = malloc(n*n*sizeof(int));
	reslutMatrix = malloc(n*n*sizeof(int));
	tempVector = malloc(n*sizeof(int));

	createMatrix(*flag, n, matrix1, matrix2);
	// if(flag == 'R'){
	// 	// createMatrix(n, matrix1, matrix2);
	// 	int row;
	// 	int colum;
	// 	int counter;
	// 	int temp =0;
	// 	srand(time(NULL));

	// 	for(counter=0; counter<2; counter++){
	// 		for(row=0; row<n; row++){
	// 			for(colum=0; colum<n; colum++){
					
	// 				temp = (rand()%10);
	// 				if(counter == 0)
	// 					matrix1[row*n+colum] = temp;
	// 				else
	// 					matrix2[row*n+colum] = temp;
	// 			}
	// 		}
	// 	}
	// }
	// else if(flag == 'I'){
	// 	reciveMatrix(n, matrix1);
	// 	reciveMatrix(n, matrix2);
	// }

	if(strcmp("ijk",*form) == 0){
		int i,j,k;
		for(i=0; i<n; i++){
			for(j=0; j<n; j++){
				tempVector[i] = 0;
				for(k=0; k<n; k++){

					reslutMatrix[i*n+j] += matrix1[i*n+k] * matrix2[k*n+j];
				}
				// printf("%i\n", tempVector[i] );
				// reslutMatrix[i*n+j] = tempVector[i];
			}
		}
	}
	else if(strcmp("ikj",*form) == 0){
		int i,j,k;
		for(i=0; i<n; i++){
			for(k=0; k<n; k++){
				tempVector[i] = 0;
				for(j=0; j<n; j++){

					reslutMatrix[i*n+j] += matrix1[i*n+k] * matrix2[k*n+j];
				}
			}
		}
	}
	else if(strcmp("kji",*form) == 0){
		int i,j,k;
		for(k=0; k<n; k++){
			for(j=0; j<n; j++){
				tempVector[i] = 0;
				for(i=0; i<n; i++){

					reslutMatrix[i*n+j] += matrix1[i*n+k] * matrix2[k*n+j];
				}
			}
		}
	}
	else
		printf("Please specifiy a correct Form");

	printMatrix(n, reslutMatrix);

	free(matrix1);
	free(matrix2);
	free(reslutMatrix);
	return 0;
}