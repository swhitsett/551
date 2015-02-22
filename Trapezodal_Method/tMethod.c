#include <stdio.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]){
	
	int alpha;
	int beta;
	int november;

	if(argc == 4){

		alpha = argv[1];
		beta = argv[2];
		november = argv[3];	
	}
	else
		printf("Input error");






	return 0;	
}