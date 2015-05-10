#include <stdio.h>
#include <stdlib.h>

int main(){

	int i;
	int j;
	int k;
	int n;
	double sum = 0.0;
	double C;
	double **A = NULL;
	double *X;
	// double C;

	scanf("%d\n", &n);

	A = malloc(n * sizeof(double));
	X = malloc(n * sizeof(double));
	// C = malloc(n * n * sizeof(double));

	// populateMatrix();
	for(i=0; i<n; i++){
		A[i] = malloc((n+1)* sizeof(double));

		for(j=0; j<(n+1); j++){
			// printf("A[%d][%d]: ", i,j);
			scanf("%lf",&A[i][j]);
		}
	}
	// printf("\n\n");
	// for (i = 0; i < n; i++) {
 //        for (j = 0; j < n+1; j++)
 //            printf("%lf ", A[i][j]);
 //        printf("\n");
 //    }

	for(j=0; j<n; j++){
		for(i=0; i<n; i++){
			if(i>j){
				C=A[i][j]/A[j][j];
				for(k=0; k<n+1; k++){
					A[i][k]=A[i][k]-C*A[j][k];
				}
			}
		}
	}

	X[n-1]=A[n-1][n]/A[n-1][n-1];

	/* this loop is for backward substitution*/
	for(i=n-1; i>0; i--)
	{
		sum=0.0;
		for(j=i+1; j<=n; j++)
		{
			sum=sum+A[i-1][j]*X[j];
			// printf("A[%d][%d] * x[%d]\n",i,j,j );
		}
		// printf("\n%lf\n",sum);
		X[i]=(A[i][n+1]-sum)/A[i][i];
		// printf("x[%d]=(A[%d][%d+1]-sum)/A[%d][%d]\n",i,i,n,i,i );
	}

	// printf("\nThe solution is: \n");
	for(i=0; i<n; i++)
	{
		// printf("\nx%d=%f\t",i,X[i]); /* x1, x2, x3 are the required solutions*/
	}
	return 0;
}

// #include <stdio.h>
// #include <stdlib.h>

// int main(){

// 	int i;
// 	int j;
// 	int k;
// 	int n;
// 	double C;
// 	double **A = NULL;
// 	double *X;
// 	// double C;

// 	scanf("%d\n", &n);

// 	A = malloc(n * sizeof(double));
// 	X = malloc(n * sizeof(double));
// 	// C = malloc(n * n * sizeof(double));

// 	// populateMatrix();
// 	for(i=0; i<n; i++){
// 		A[i] = malloc((n+1)* sizeof(double));

// 		for(j=0; j<(n+1); j++){
// 			printf("A[%d][%d]: ", i,j);
// 			scanf("%lf",&A[i][j]);
// 		}
// 	}
	
// 	printf("\n\n");
// 	for (i = 0; i < n; i++) {
//         for (j = 0; j < n+1; j++)
//             printf("%lf ", A[i][j]);
//         printf("\n");
//     }

// 	//calculate matrix
// 	for(j=0; j<n; j++){
// 		for(i=0; i<n; i++){
// 			if(i >= j){
// 				C = A[i][j] / A[j][j];
// 				for(k=0; k<(n); k++){
// 					A[i][k] = A[i][k] - C * A[j][k];
// 				}
// 			}
// 		}
// 	}
	
// 	X[n-1] = A[n-1][n] / A[n-1][n-1];
// 	for(i = n - 1; i >= 0; i--)
//     {
//         X[i] = A[i][n] / A[i][i];
//         for(j = i - 1; j >= 0; j--)
//         {
//             A[j][n] = A[j][i] * X[i];
//         }
//     }
// 	// for(i=n-1; i>0; i--){
// 	// 	int sum=0;
// 	// 	for(j=i+1; j<n; j++){
// 	// 		sum=sum+A[i][j]*X[j];
// 	// 	}
// 	// 	X[i]=(A[i][(n+1)]-sum)/A[i][i];
// 	// }

	

// 	for(i=0; i<n; i++)
// 	{
// 		printf("\nx%d=%lf\t",i,X[i]); /* x1, x2, x3 are the required solutions*/
// 	}

// 	return 0;
// }