//----------------------------------------------------------------------
// #include <stdio.h>
// #include <stdlib.h>
// // #include <pthread.h>
// #include <cmath>

// int a = 100;
// int b = 600;
// unsigned long n = 1000000;
// char comp[15];
// char comp2[15];
// char compx[7];
// double actualNum = 4003.7209001513;//4003.71980545360157

// double result = 0;
// double approx = 0;

// double f(double x) {
//    return cos(x/3.0) - 2 * cos(x/5.0) + 5 * sin(x/4.0) + 8;
// }

// void tFunction() {
//    double h = (double)(b-a)/(double)n;
//    int i;
//   for(i=1; i<n; i=i*2){ 
//     int run = 0;
//     // printf("%i\n",i);
//     for(run = 1; run < i; run++) {
//      double y = a + run * h;
//      approx += f(y);
//      double yarp = approx*h;
//      snprintf(comp,19,"%.2f",stopPoint);
//      snprintf(comp,19,"%.10f",yarp);
//      snprintf(comp2,19,"%.10f",actualNum);
//      // printf("%.10f\n",approx*h);
//      if(yarp == actualNum){
//       printf("YAY");
//       break;
//     }
//   }
   
//    } 
//    result += approx * h;
//    printf("num of Trapazoids: %i\n", i);
//    printf("final result: %.14f\n", result);
// }

// int main() {
//    pthread_t thread;
//    int x;
//    approx = (f(a) + f(b)) / 2.0;
//    tFunction();

//    // printf("RESULT: %f\n", result);
// }
//--------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <cmath>

int a = 100;
int b = 600;
int numOfTraps = 0;
unsigned long n = 3000000;
double actualNum = 4003.7209001513;
double result;
double approx;
double h;//= (double)(b-a)/(double)n;
char withinRange[7];
char guessedNumber[15];
char desiredNumber[15];
bool breakOut = false;


double f(double x) {
  return cos(x/3.0) - 2 * cos(x/5.0) + 5 * sin(x/4.0) + 8;
}

void oneByone(int unox2){

  for(int uno=unox2; uno<n; uno++){
    for(int i=1; i<uno; i++){
      result = 0;
      double y = a + i*h;
      approx += f(y);
      result += approx*h;
      // snprintf(guessedNumber,19,"%.10f",result);
      // snprintf(desiredNumber,19,"%.10f",actualNum);
      snprintf(withinRange,19,"%.0f",result);
      printf("%f\n",result);
      printf("%i\n",uno);
      break;
      breakOut = true;

      if(result == actualNum){
        printf("yay");
        breakOut = true;
        break;
      }

      if(guessedNumber == desiredNumber){
        breakOut = true;
        break;
      }

    }
    break;
  }
}
void tFunction(){

  // double result;
  // double approx = (f(a) + f(b)) / 2.0;
  // double h = (double)(b-a)/(double)n;

  for(numOfTraps=1; numOfTraps<n; numOfTraps=numOfTraps*2){
    for(int i=1; i<numOfTraps; i++){
      result = 0;
      double y = a + i*h;
      approx += f(y);
      result += approx*h;
      // snprintf(guessedNumber,19,"%.10f",result);
      // snprintf(desiredNumber,19,"%.10f",actualNum);
      snprintf(withinRange,19,"%.0f",result);
      // printf("%s\n",withinRange);

      if(result >= 4003.72){
        printf("yay1\n");
        oneByone(numOfTraps);
        breakOut = true;
        break;
      }

      if(guessedNumber == desiredNumber){
        breakOut = true;
        break;
      }

    }
    if(breakOut == true)
      break;
  }

}

int main(){
  approx = (f(a) + f(b)) / 2.0;
  tFunction();
}