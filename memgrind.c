#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "mymalloc.h"

struct timeval start;
struct timeval stop;

//malloc 1 byte and immediately free it - 120x
int testA(){
	gettimeofday(&start, 0);
	for (int i = 0; i < 120; i++) {	
		char* ptr = (char*) malloc(sizeof(char));
		free(ptr);
	}
	gettimeofday(&stop, 0);
	return (( stop.tv_sec-start.tv_sec)*1000000 + stop.tv_usec-start.tv_usec);
}

//malloc 1 byte 120x, then free all the pointers
int testB(){
	gettimeofday(&start, 0);
	char* arrayP[120];	
	for (int i = 0; i < 120; i++) {	
		arrayP[i] = (char*) malloc(sizeof(char));
	}
	for (int i = 0; i < 120; i++) {	
		free(arrayP[i]);
	}
	gettimeofday(&stop, 0);
	return (( stop.tv_sec-start.tv_sec)*1000000 + stop.tv_usec-start.tv_usec);
}

//test C: isRandByte = 0
	//randomly choose between malloc or free 1 byte
	//do this 120x
//test D: isRandByte = 1
	//same as test C, except randomly choose n bytes to malloc/free
	//0 <= n <= 31
int testCD(int isRandByte) {
	gettimeofday(&start, 0);
	char* arrayP[120];
	int mallocIndex = 0;
	int freeIndex = 0; 	
	int byteSize = 1;

	//loop until malloc'd 120x and free'd 120x
	while (mallocIndex < 120 || freeIndex < 120) {
		//mOrF chooses between malloc or free
		//and for part D also decides number of blocks to malloc
		int mOrF = rand();
		//if mOrF is odd and we have not malloc'd 120x, perform malloc
		if (mOrF % 2 && mallocIndex < 120) {
			//if part D, mOrF is byteSize to malloc
			if (isRandByte) { byteSize = mOrF % 32; }
			arrayP[mallocIndex] = (char*) malloc(byteSize);
			//if malloc returned NULL, do not increase times malloc'd
			if (arrayP[mallocIndex] != NULL) {
				++mallocIndex;
			}
		}
		//if mOrF is even and we have something to free: perform free
		else if (freeIndex < mallocIndex && mallocIndex != 0) {
			free(arrayP[freeIndex]);
			arrayP[freeIndex] = NULL;
			++freeIndex;
		}
	}
	gettimeofday(&stop, 0);
	return (( stop.tv_sec-start.tv_sec)*1000000 + stop.tv_usec-start.tv_usec);
}

//malloc 50 bytes 75 times for total of 4000 bytes
//randomly choose 50 byte blocks to free
//on each free, split block in half,
//and malloc both blocks making sure all the bytes in the 50byte block are occupied
//free all mallocs
int testE() {
	gettimeofday(&start, 0);
	char* arrayP1[75];
	char* arrayP2[75];
	
	//malloc 75 50byte blocks
	for (int i =0; i < 75; i++) {
		arrayP1[i]= (char*)malloc(50);
		arrayP2[i] = NULL;
	}

	//free each 50 byte block and split them in 2 different malloc'd blocks
	for (int i =0; i < 75; i++) {
		free(arrayP1[i]);
		arrayP1[i] = (char*) malloc(24);
		arrayP2[i] = (char*) malloc(24); 
	}
	
	//free all the blocks
	for (int i = 0; i < 75; i++) {
		free(arrayP1[i]);
		free(arrayP2[i]);
	}
	gettimeofday(&stop, 0);
	return (( stop.tv_sec-start.tv_sec)*1000000 + stop.tv_usec-start.tv_usec);
}

//print average time for each workload to perform
int main() {
	int timeA = 0;
	int timeB = 0;
	int timeC = 0;
	int timeD = 0;
	int timeE = 0;
	for (int i = 0; i < 50; i++) {
		timeA += testA();
		timeB += testB();
		timeC += testCD(0);
		timeD += testCD(1);
		timeE += testE();
	}
	printf("Mean time in microseconds for...\n");
	printf("Workload A: %d\n", timeA/50);
	printf("Workload B: %d\n", timeB/50);
	printf("Workload C: %d\n", timeC/50);
	printf("Workload D: %d\n", timeD/50);
	printf("Workload E: %d\n", timeE/50);

	return 0;
}
