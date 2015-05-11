#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <time.h>
// #include <omp.h>
void upperTriangular(double** A, int n, int threads){
	
	int i, j, k, curMax;
	double pivot, multiplier;
	double max = 0.0;

	#pragma omp parallel num_threads(threads) \
        	  default(none) shared(A, n, pivot, max_iter, max) 
        	  private(i, j, k, multiplier)
	for(i = 0; i < n; i++){
        // partial pivot
		for(j = i; j < n; j++){
			if(A[j][i] > max){
				max = A[i][j];
				curMax = j;
			}
		}
		#pragma omp for
		for(k = i; k < n + 1; k++){
			pivot = A[curMax][k];
			A[curMax][k] = A[i][k];
			A[i][k] = pivot;
		}
		#pragma omp for
		for(k = i + 1; k < n; k++){
			multiplier = -(A[k][i] / A[i][i]);
			for(j = i; j < n + 1; j++){
				A[k][j] = A[k][j] + multiplier * A[i][j];
			}
		}	
	}
}

void backwardSub(double** A, double* X, int n, int threads){
	int i;
	int j;
	double sum;
	# pragma omp parallel num_threads(threads) \
        default(none) shared(A, X, n) private(i, k)
	for(i=n-1; i>=0; i--){
		sum=0.0;
		#pragma omp for
		for(j=i+1; j<=n-1; j++){
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
	srand48(time(NULL));

	for(i=0; i<n; i++){
		A[i] = malloc((n+1)* sizeof(double));

		for(j=0; j<(n+1); j++){
			// if(j == (n+1))
				// C[i] = drand48();
			// else
				A[i][j] = drand48() * 2e6 - 1e6;

			// scanf("%lf",&A[i][j]);
		}
	}
}

int main(int argc, char *argv[]){

	int i;
	int j;
	int k;
	int n;
	int threads;
	double start, finished, elapsed;
	double **A = NULL;
	double *X;
	// double C;

	if(argc == 3){
		n = atoi(argv[1]);
		threads = atoi(argv[2]);
	}

	// scanf("%d\n", &n);

	A = malloc(n * sizeof(double));
	X = malloc(n * sizeof(double));
	// C = malloc(n * n * sizeof(double));

	populateMatrix(A, n);
	// printf("\n\n");
	// for (i = 0; i < n; i++) {
 //        for (j = 0; j < n+1; j++)
 //            printf("%lf ", A[i][j]);
 //        printf("\n");
 //    }
	// for(i=0; i<n; i++){
	// 	A[i] = malloc((n+1)* sizeof(double));

	// 	for(j=0; j<(n+1); j++){
	// 		// printf("A[%d][%d]: ", i,j);
	// 		scanf("%lf",&A[i][j]);
	// 	}
	// }
	start = omp_get_wtime();
	upperTriangular(A, n, threads);
 // 	printf("\n\n");
	// for (i = 0; i < n; i++) {
 //        for (j = 0; j < n+1; j++)
 //            printf("%lf ", A[i][j]);
 //        printf("\n");
 //    }
	// X[n-1]=A[n-1][n]/A[n-1][n-1];
	backwardSub(A, X, n, threads);
	finished = omp_get_wtime();
	elapsed = finished - start;
	for(i=0; i<n; i++)
		printf("\nx%d=%f\t",i,X[i]);

	printf("\nusinig %d cores\n", omp_get_num_procs());
	printf("usinig %d threads\n", omp_get_num_threads());
	printf("elapsed time = %f seconds\n", elapsed);

	free(A);
	free(X);
	return 0;
}
