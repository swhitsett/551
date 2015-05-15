#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <omp.h>

int threads;
int threads_used;

void createMatrix(double** A, double* B, 
                  double** initMatrix,
                  double* initVec, int n){
    int i;
    int j;
    double tmp;
    srand48(time(NULL));

    for(i=0; i<n; i++){
        A[i] = malloc(n * sizeof(double));
        initMatrix[i] = malloc(n * sizeof(double));

        for(j=0; j<(n+1); j++){
            tmp = drand48() * 2e6 - 1e6;
            A[i][j] = tmp;
            initMatrix[i][j] = tmp;
        }
        tmp = drand48() * 2e6 - 1e6;
        B[i] = tmp;
        initVec[i] = tmp;
    }
}

void upperTriangular(double **A, double * B, int n){
    int i;
    int j;
    int k;
    int greatest;
    double tmp;
    double tmp2;
    double maxVal;
    
    for(i = 0; i < n-1;i++) {
        
        greatest = i;

        #pragma omp parallel for num_threads(threads) default(none) \
                       shared(n, A, B, i, greatest) private(maxVal, j)
        for(j = i; j < n; j++) {
            if(fabs(A[j][i]) > fabs(maxVal)){
                maxVal = A[j][i];
                greatest = j;
            }
        }

        for(k = i; k < n; k++){
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

void backSub(double** A, double* B, double* C, int n){

    int i;
    int j;
    for(i = n-1; i >= 0; i--) {
        C[i] = B[i] / A[i][i];
        #pragma omp parallel for num_threads(threads) default(none) \
                shared(A, B, n, i, C, threads_used) private(j)
        for(j = n-1; j >= 0; j--) {
            B[j] -= A[j][i] * C[i];
            if(j == 0)             
                threads_used = omp_get_num_threads();
        }
    }
}

void computeResidual(double** initMatrix, double* initVec,
                     double* C, double* X, int n){

    int i;
    int j;
    #pragma omp parallel for num_threads(threads) default(none) \
        shared(initMatrix, initVec, C, n, X) private(i, j)
    for(i = 0; i < n; i++) {
        X[i] = 0.0;
        for(j = 0; j < n; j++) {
            X[i] += initMatrix[i][j] * C[j];
        }
        X[i] = initVec[i] - X[i];
    }
}

void l2norm(double* X, double* L2, int n){
    
    double result = 0.0;
    int i;
    #pragma omp parallel for num_threads(threads) default(none) \
        shared(result, X, n) private(i)
    for(i = 0; i < n; i++) {
        result += pow(X[i], 2);
    }
    *L2 = sqrt(result);
}

int main(int argc, char* argv[]){
    int n;
    // int i;
    // int j;
    double start, finished, elapsed;
    double** initMatrix;
    double* initVec;
    double** A;
    double* B;
    double* C;
    double* X;
    double* L2;
    
    if(argc == 3) {
        n = atoi(argv[1]);
        threads = atoi(argv[2]);
    }
    else{
        printf("invalid input");
        return 0;
    }
    
    initMatrix = malloc(n * sizeof(double*));
    initVec = malloc(n * sizeof(double));
    A = malloc(n * sizeof(double*));
    B = malloc(n * sizeof(double));
    C = malloc(n * sizeof(double));
    X = malloc(n * sizeof(double));
    L2 = malloc(sizeof(double));


    createMatrix(A, B, initMatrix, initVec, n);
    
    start = omp_get_wtime();

    upperTriangular(A, B, n);
    backSub(A, B, C, n);
    computeResidual(initMatrix, initVec, C, X, n);

    finished = omp_get_wtime();
    elapsed = finished - start;

    l2norm(X, L2, n);

    printf("Processors used: %d\n", omp_get_num_procs());
    printf("Threads used: %d\n", threads_used);
    printf("Elapsed time: %f\n", elapsed);
    printf("L2 norm: %f\n", *L2);
    return 0;
}
