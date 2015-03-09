// #include <stdio.h>
// #include <stdio.h>
// // #include <pthread.h>
// #include <cmath>
// #include <regex.h>
// // #include <mpi.h>

// int i;
// int a = 600;
// int b = 100;
// int n = 1000000;
// double result = 0;

// double f(double x){
// 	return cos(x/3.0)-(2.0*cos(x/5.0))+(5.0*sin(x/4.0))+8.0;
// }

// void trap(){

// 	double h = ((double)b - (double)a)/(double)n;
// 	double approx = (f((double)a) + f((double)b))/2.0;

// 	for(i=2000000; i <= 3000000; i++){
// 		double x_i = (double)a + (double)i*h;
// 		approx += f(x_i);
  
// 		printf("N=%d Result is =%.14f \n", i, approx*h);
// 	}

// 	// approx = h*approx;	
// 	// printf("N=%d Result is =%.40f",n,approx);	
// }

// int main(){
// 	// pthread_t t;
// 	// for(int i=0; i<8; i++){
// 	// 	pthread_create(&t, NULL, trap, (void*)x);
// 	// }
// 	// pthread_exit(NULL);
//   trap();
// 	return 0;
// }
//--------------------------------g++ tMethod.c -lpthread----------------------
// #include <stdio.h>
// #include <stdlib.h>
// #include <pthread.h>
// #include <cmath>

// int a = 100;
// int b = 600;
// unsigned long n = 3000000;

// double result = 0;
// double approx = 0;

// double f(double x) {
//    return cos(x/3.0) - 2 * cos(x/5.0) + 5 * sin(x/4.0) + 8;
// }

// void * threadFunction(void* vargp) {
//    double h = (double)(b-a) / (double)n;
//    double curResult = approx;
//    int *id = (int*)vargp;
//    unsigned long end = (n/8)*((*id) + 1);
//    printf("thread: %d on: %ul\n", id, end);
//    unsigned long i;
//    for(i = 0; i < end; i++) {
//        double y = a + i * h;
//        curResult += f(y);
//        // printf("N=%d Result is =%.14f \n", i, curResult*h);    
//    } 
//    result += curResult * h;
//    printf("curResult: %f\n", curResult * h);
//    printf("Result is = %.14f\n", result);
// }

// int main() {
//    pthread_t thread;
//    int x;
//    approx = (f(a) + f(b)) / 2.0;
//    for(x = 0; x < 8; x++) {
//        pthread_create(&thread, NULL, threadFunction, &x);
//    }
//    pthread_exit(NULL);
//    printf("RESULT: %f\n", result);
// }
//----------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
// #include <pthread.h>
#include <cmath>

int a = 100;
int b = 600;
unsigned long n = 1000000;
char comp[15];
char comp2[15];
double actualNum = 4003.7209001513;//4003.71980545360157

double result = 0;
double approx = 0;

double f(double x) {
   return cos(x/3.0) - 2 * cos(x/5.0) + 5 * sin(x/4.0) + 8;
}

void tFunction() {
   double h = (double)(b-a) / (double)n;
   // double localResult = approx;
   // int id = *(int *)vargp;
   // printf("%d", *(int*)vargp);
   // unsigned long end = (n/8)*(id + 1);
   // printf("thread %d works up to %u\n", id, end);
   int i;
  for(i=1; i<n; i++){ 
    int run = 0;
    for(run = 1; run < i; run++) {
     double y = a + run * h;
     approx += f(y);
     double yarp = approx*h;
     snprintf(comp,19,"%.10f",yarp);
     snprintf(comp,19,"%.10f",actualNum);
     // printf("%.10f\n",approx*h);
     if(yarp == actualNum){
      printf("YAY");
      break;
    }
  }
       // break;
       // if()    
   } 
   result += approx * h;
   printf("num of Trapazoids: %i\n", i);
   printf("final result: %.14f\n", result);
}

int main() {
   pthread_t thread;
   int x;
   approx = (f(a) + f(b)) / 2.0;
   tFunction();

   // printf("RESULT: %f\n", result);
}
//--------------------------------------------------------------------