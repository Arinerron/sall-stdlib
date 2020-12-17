#include "types.h"

typedef struct chunk_meta {
    size_t size;
    void *freeptr;
} chunk_meta;

#define _MAX_ADDRESS 0xFFFF

void *free_ptr_head = NULL;
void *bottom_ptr = (void *)_MAX_ADDRESS;

#define IS_CHUNK_FREE(chunk) (!(chunk)->freeptr)
#define GET_CHUNK_META(ptr) (chunk_meta *) ((ptr) - sizeof(chunk_meta))

