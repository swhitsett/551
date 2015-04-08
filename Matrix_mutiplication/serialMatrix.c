#include <stdio.h>
#include <stdlib.h>

void reciveMatrix(int n,int matrix[]);

void reciveMatrix(int n, int matrix[] ){
	int row;
	int colum;
	for(row=0; row<n; row++){
		for(colum=0; colum<n; colum++){
			scanf("%i", &matrix[row*n+colum]);
		}
	}
}

int main(){

	char flag;
	char form[3];
	int *matrix1;
	int *matrix2;
	int n;

	scanf("%s %c %d",form, &flag, &n);

	matrix1 = malloc(n*n*sizeof(int));
	matrix2 = malloc(n*n*sizeof(int));

	reciveMatrix(n, matrix1);
	reciveMatrix(n, matrix2);
	
	// for printing pourpses --------------
	// int i;
	// int j;
	// for (i = 0; i < n; i++) {
	//      for (j = 0; j < n; j++)
	//         printf("%d ", matrix1[i*n+j]);
	//      printf("\n");
	//   	}

	free(matrix1);
	free(matrix2);
	return 0;
}