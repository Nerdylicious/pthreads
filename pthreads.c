/**
 * pthreads.c
 *
 * PURPOSE: 	This program create some threads, each randomly choosing numbers
 * 			from an array, and each thread computing an average from it's set
 *			of random numbers. 
 *
 * TO COMPILE:	gcc -Wall pthreads.c -o prog -lpthread
 * TO RUN:	./prog
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>


#define MAX_LENGTH 50
#define AR_LENGTH 10000

struct ARGUMENT
{
	int m_NumElts;
	int m_NumRead;
	int m_ar[AR_LENGTH];
	
};

typedef struct ARGUMENT Argument;

/**
 * PURPOSE: For each iteration of the loop, it generates a random number.
 *		  This random number is used to index one of the numbers in the
 *          array ar. We add the number indexed from ar to the total and 
 *          at the end of the loop computes an average from the set of 
 *          randomly selected numbers from ar.
 
 * INPUT PARAMETERS:
 *    	Argument *ptr		This is a pointer to the Argument structure 
 *                            which contains arguments that are needed in 
 *                            this routine.
 
 * OUTPUT PARAMETERS:
 *    	None, the thread exits at the end of this routine.
 *
 */
void *DoAve(void *arg){

	//cast from void pointer to our struct type, this gets rid of warning
	Argument *ptr = (Argument*)arg;
	
	int total = 0;
	int randnum;
	int number;
	int average;
	int i;
	

	for(i = 0; i < ptr->m_NumElts; i ++){
	
		//generate a random number between 0 and NumRead-1 (9999)	
		randnum = rand() % ptr->m_NumRead;
		number = ptr->m_ar[randnum];
		total += number;

	
	}
	//compute the integer average of numbers randomly chosen from ar
	average = total/ptr->m_NumElts;
	printf("\nThread pid: %lu total: %d Average: %d\n", pthread_self(), total, average);
	
	
	//generate a random number between 0 and 2 and
	//sleep for this number of seconds
	int sleep_time = rand() % 3;
	sleep(sleep_time);
	

	pthread_exit(NULL);
}

int main (int argc, char * argv[]) {

	char number[MAX_LENGTH];
	int ar[AR_LENGTH];
	int NumRead = 0;
	int rc;
	Argument arg;
	
	//seed the rand function each time so we get a new sequence of random
	//numbers for each run of the program
	//do this at the start of each run of the program
	srand(time(NULL));
	
	if (argc != 3){
        fprintf(stderr,"Usage: a.out NumThreads NumElts\n");
        exit(EXIT_FAILURE);
    	}
    	
    	int NumThreads = atoi(argv[1]);
    	int NumElts = atoi(argv[2]);
    	pthread_t threads[NumThreads];

	FILE *in;
	in = fopen("numbers.txt", "r");
	
	if(in == NULL){
		printf("Unable to open numbers.txt\n");
		exit(EXIT_FAILURE);
	}

	while(fgets(number, MAX_LENGTH, in) != NULL){
		
		ar[NumRead] = atoi(number);
		//also copy the values into ar in the struct
		arg.m_ar[NumRead] = ar[NumRead]; 
		NumRead ++;
		
	}


    	arg.m_NumElts = NumElts;
    	arg.m_NumRead = NumRead;
    	

    	int i;
    	for(i = 0; i < NumThreads; i ++){
    		//printf("Creating thread number %d\n", i);
    		//make void pointer to struct
    		rc = pthread_create(&threads[i], NULL, DoAve, (void *)&arg);
    		
    		if (rc){
         		printf("ERROR: rc is %d\n", rc); 
         		exit(EXIT_FAILURE);
      	} else {
         		//printf("\nSuccessfully created thread with id <%lu>\n", threads[i]);
      	}
    	
    	
    	}


	pthread_exit(NULL);
	
	
	

	
} // end main











