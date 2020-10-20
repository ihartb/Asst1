#ifndef _MYMALLOC_H_
#define _MYMALLOC_H_

#define malloc(x) mymalloc( x, __FILE__, __LINE__ )
#define free(x) myfree( x, __FILE__, __LINE__ )
#define MEMSIZE 4096

//metadata is represented as an unsigned short
//it tells us the amount of bytes (following the metadata block) 
//which are allocated for  user data
//if the leftmost bit is 1, then the block is occupied
//else it is free
struct _metadata_ {
	unsigned short size;
} typedef metadata;


void* mymalloc(size_t size, char* file, int line);
void myfree(void* p, char* file, int line);
void printMemory();

#endif
