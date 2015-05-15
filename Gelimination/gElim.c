#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <time.h>

int threads_used;
int threads;

void upperTriangular(double** A, int n)
{
    int i;
    int j;
    int k;
    int max_iter;
    double pivot;
    double multiplier;

    for(i = 0; i < n; i++) 
    {
        // partial pivot
        for(j = i; j < n; j++)
        {
            double max = 0.0;
            if(fabs(A[j][i]) > fabs(max))
            {
                max = A[j][i];
                max_iter = j;
            }
        }

        for(k = i; k < n + 1; k++)
        {
            pivot = A[max_iter][k];
            A[max_iter][k] = A[i][k];
            A[i][k] = pivot;
        }

#pragma omp parallel for num_threads(threads) \
        default(none) shared(A, n, i, threads) private(j, k, multiplier)

        for(k = i + 1; k < n; k++)
        {
            multiplier = A[k][i] / A[i][i];
            for(j = i; j < n + 1; j++)
            {
                A[k][j] = multiplier * A[i][j];

            }
        }
    }
}
void backwardSub(double** A, double* X, int n){
    int i;
    int j;
    for(i=n-1; i>=0; i--){
        X[i] = A[i][n] / A[i][i];
        #pragma omp parallel for num_threads(threads) \
                default(none) shared(A, n, i, X, threads_used) private(j)
        for(j=i-1; j>=0; j--){
            A[j][n] -= A[j][i] * X[i];
            if(i == 2)
               threads_used = omp_get_num_threads();
        }
    }
}

void l2_norm(double** Original, double* b, double* c, int n)
{
    int i, j;
    double x = 0.0;
    double* residual;

    residual = (double*)malloc(sizeof(double) * n);
    for(i = 0; i < n; i++)
    {
        residual[i] = 0.0;
        for(j = 0; j < n; j++)
        {
            residual[i] += Original[i][j] * c[j];
        }
        residual[i] = b[i] - residual[i];
        printf("residuals = %lf\n", residual[i]);
    }


    for(i = 0; i < n; i++)
    {
        x += (residual[i] * residual[i]);
    }
    double result = sqrt(x);
    printf("The result in l2-norm is %f\n", result);
}

void populateMatrix(double** A, int n){
    int i;
    int j;
    srand48(time(NULL));

    for(i=0; i<n; i++){
        A[i] = malloc((n+1)* sizeof(double));

        for(j=0; j<(n+1); j++){
            A[i][j] = drand48() * 2e6 - 1e6;
        }
    }
}


int main(int argc, char *argv[]){

    int n;
    double start, finished, elapsed;
    double **A = NULL;
    double *X;
    double *C;

    if(argc == 3){
        n = atoi(argv[1]);
        threads = atoi(argv[2]);
    }

    // scanf("%d\n", &n);

    A = malloc(n * sizeof(double*));
    X = malloc(n * sizeof(double));
    C = malloc(n * sizeof(double));

    populateMatrix(A, n);
    start = omp_get_wtime();
    upperTriangular(A, n);
    backwardSub(A, X, n);
    finished = omp_get_wtime();
    elapsed = finished - start;
      
    l2_norm(A, X, C, n);
//    for(i=0; i<n; i++)
//        printf("\nx%d=%f\t",i,X[i]);

    printf("\ncores: %d\n", omp_get_num_procs());
    printf("threads: %d\n", threads_used);
    printf("elapsed time = %f seconds\n", elapsed);

    free(A);
    free(X);
    return 0;
}
