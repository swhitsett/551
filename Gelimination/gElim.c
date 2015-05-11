#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
// #include <omp.h>
void upperTriangular(double** A, int n, int threads){
	
	int i;
	int j;
	int k;
	double C;

	for(j=0; j<n; j++){
		for(i=0; i<n; i++){
			if(i>j){										//selection of pivot
				C=A[i][j]/A[j][j];
				for(k=0; k<n+1; k++){
					A[i][k]=A[i][k]-C*A[j][k];
				}
			}
		}
	}
}

void backwardSub(double** A, double* X, int n, int threads){
	int i;
	int j;
	double sum;
	/* this loop is for backward substitution*/
	for(i=n-1; i>=0; i--)
	{
		sum=0.0;
		for(j=i+1; j<=n-1; j++)
		{
			// printf("%lf and %lf\n", A[i][j], X[j]);
			sum=sum+A[i][j]*X[j];
			// printf("sum= %lf A[%d][%d]= %lf X[%d]= %lf\n", sum, i, j, A[i][j], j, X[j]);
		}
		X[i]=(A[i][n]-sum)/A[i][i];
		// printf("x[%d]=(A[%d][%d]-%lf)/A[%d][%d]\n",i,i,n,sum,i,i );
	}
}

void populateMatrix(double** A, int n){
	int i;
	int j;

	for(i=0; i<n; i++){
		A[i] = malloc((n+1)* sizeof(double));

		for(j=0; j<(n+1); j++){
			// if(j = (n+1))
			// 	C[i] = drand48();
			// else
			// 	A[i][j] = drand48();

			scanf("%lf",&A[i][j]);
		}
	}
}

int main(int argc, char *argv[]){

	int i;
	int j;
	int k;
	int n;
	int threads;
	// double sum = 0.0;
	// double C;
	double **A = NULL;
	double *X;
	// double C;

	if(argc == 3){
		n = atoi(argv[1]);
		threads = atoi(argv[2]);
	}

	scanf("%d\n", &n);

	A = malloc(n * sizeof(double));
	X = malloc(n * sizeof(double));
	// C = malloc(n * n * sizeof(double));

	populateMatrix(A, n);

	// for(i=0; i<n; i++){
	// 	A[i] = malloc((n+1)* sizeof(double));

	// 	for(j=0; j<(n+1); j++){
	// 		// printf("A[%d][%d]: ", i,j);
	// 		scanf("%lf",&A[i][j]);
	// 	}
	// }

	upperTriangular(A, n, threads);

	// X[n-1]=A[n-1][n]/A[n-1][n-1];
	backwardSub(A, X, n, threads);

	for(i=0; i<n; i++)
		printf("\nx%d=%f\t",i,X[i]);

	free(A);
	free(X);
	return 0;
}
