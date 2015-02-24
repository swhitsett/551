#include <stdio.h>
#include <stdio.h>
#include <cmath>
// #include <mpi.h>

double f(double x){
	return cos(x/3.0)-(2.0*cos(x/5.0))+(5.0*sin(x/4.0))+8.0;
}

int main(int argc, char *argv[]){
	
	int a = 600;
	int b = 100;
	int n = 1000000;
	double h = ((double)b - (double)a)/(double)n;
	double approx = (f((double)a) + f((double)b))/2.0;

	// if(argc == 4){

	// 	alpha = argv[1];
	// 	beta = argv[2];
	// 	november = argv[3];	
	// }
	// else
	// 	printf("Input error");

	for(int i=0; i <= 1; i++){
		double x_i = (double)a + (double)i*h;
		approx += f(x_i);
		printf("N=%d Result is =%.14f \n", i, approx*h);
	}

	// approx = h*approx;	
	// printf("N=%d Result is =%.40f",n,approx);



	return 0;	
}