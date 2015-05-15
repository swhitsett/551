#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <time.h>

int threads;
int thread_count;

void createMatrix(double** A, double* B, 
                  double** originalMatrix,
                  double* originalVector, int n){
    int i;
    int j;
    double tmp;
    srand48(time(NULL));

    for(i=0; i<n; i++){
        A[i] = malloc(n * sizeof(double));
        originalMatrix[i] = malloc(n * sizeof(double));

        for(j=0; j<(n+1); j++){
            tmp = drand48() * 2e6 - 1e6;
            A[i][j] = tmp;
            originalMatrix[i][j] = tmp;
        }
        tmp = drand48() * 2e6 - 1e6;
        B[i] = tmp;
        originalVector[i] = tmp;
    }
}

void upperTriangular(double **A, double * B, int n){
    int i;
    int j;
    int k;
    double tmp;
    double tmp2;
    double maxVal;
    
    for(i = 0; i < n-1;i++) {
        int greatest = i;

        // #pragma omp parallel for num_threads(threads) default(none) \
         43                 shared(n, A, B, i, greatest) private(j)
        for(j = i; j < n; j++) {
            if(fabs(A[j][i]) > fabs(maxVal)){
                maxVal = A[j][i];
                greatest = j;
            }
        }

        for(k = i; k < n; k++)
        {
            tmp = A[greatest][k];
            A[greatest][k] = A[i][k];
            A[i][k] = tmp;
        }
        tmp2 = B[i];
        B[i] = B[greatest];
        B[greatest] = tmp2;

        #pragma omp parallel for num_threads(threads) default(none) \
                shared(A, B, i, n) private(j, k)
        for(j = i+1; j < n; j++) {
            double multiplier = A[j][i] / A[i][i];
            for(k = i; k < n; k++) {
                A[j][k] -= multiplier * A[i][k];
            }
            B[j] -= multiplier * B[i];
        }
    }
}

void backSubstitute(double** A, double* B, double* C, int n){

    int i;
    int j;
    for(i = n-1; i >= 0; i--) {
        C[i] = B[i] / A[i][i];
        #pragma omp parallel for num_threads(threads) default(none) \
                shared(A, B, n, i, C, thread_count) private(j)
        for(j = n-1; j >= 0; j--) {
            B[j] -= A[j][i] * C[i];
            if(i == 0 && j == 0) 
                thread_count = omp_get_num_threads();
        }
    }
}

void computeResidual(double** originalMatrix, double* originalVector,
                     double* C, double* X, int n){

    int i;
    int j;
    // double* result = malloc(sizeof(double) * n);
    #pragma omp parallel for num_threads(threads) default(none) \
        shared(originalMatrix, originalVector, C, n, X) private(i, j)
    for(i = 0; i < n; i++) {
        X[i] = 0.0;
        for(j = 0; j < n; j++) {
            X[i] += originalMatrix[i][j] * C[j];
        }
        X[i] = originalVector[i] - X[i];
    }
    // return result;
}

double l2norm(double* X, int n){

    double result = 0;
    int i;
    #pragma omp parallel for num_threads(threads) default(none) \
        shared(result, X, n) private(i)
    for(i = 0; i < n; i++) {
        result += pow(X[i], 2);
    }
    return sqrt(result);
}

int main(int argc, char* argv[]){
    int n;
    // int i;
    // int j;
    double start, finished, elapsed;
    double** A;
    double* B;
    double* C;
    double* X;
    
    if(argc == 3) {
        n       = atoi(argv[1]);
        threads = atoi(argv[2]);
    }
    else{
        printf("invalid input");
        return 0;
    }
    
    double** originalMatrix = malloc(n * sizeof(double*));
    A              = malloc(n * sizeof(double*));
    double* originalVector  = malloc(n * sizeof(double));
    B               = malloc(n * sizeof(double));
    C               = malloc(n * sizeof(double));
    X               = malloc(n * sizeof(double));


    createMatrix(A, B, originalMatrix, originalVector, n);
    
    start = omp_get_wtime();

    upperTriangular(A, B, n);
    backSubstitute(A,B,C,n);
    computeResidual(originalMatrix, originalVector, C, X, n);

    finished = omp_get_wtime();
    elapsed = finished - start;

    printf("Procs used:        %d\n", omp_get_num_procs());
    printf("Threads used:      %d\n", thread_count);
    printf("Elapsed time:      %f\n", elapsed);
    printf("Residual l^2 norm: %f\n", l2norm(X, n));
    return 0;
}
