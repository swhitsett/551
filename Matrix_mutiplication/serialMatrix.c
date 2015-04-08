#include <stdio.h>
#include <stdlib.h>

void reciveMatrix(int,int);

void reciveMatrix(int n, int matrix ){
	int row;
	int colum;
	for(row=0; row<n; row++){
		for(colum=0; colum<n; colum++){
			scanf("%i", matrix[row*n+colum]);
		}
	}
}

int main(){

	int *matrix1;
	int *matrix2;
	char form[3];
	char flag;
	int n;

	scanf("%s %c %d",form, &flag, &n);

	matrix1 = malloc(n*n*sizeof(int));
	matrix2 = malloc(n*n*sizeof(int));

	reciveMatrix(n, &matrix1);
	reciveMatrix(n, &matrix2);

	free(matrix1);
	free(matrix2);
	return 0;
}