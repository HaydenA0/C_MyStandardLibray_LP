
#include <unistd.h>
#ifndef MY_ALLOCATER_H
#define MY_ALLOCATER_H
#define TRUE 1
#define FALSE 0

typedef struct MemoryChunk
{
    int is_free; // TRUE or FALSE
    long unsigned size;
    struct MemoryChunk *next_chunk;
    struct MemoryChunk *prev_chunk;
} MemoryChunk;

void *my_allocate(size_t requested_size);
void my_free(void *pointer);
#endif
