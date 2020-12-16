#include <panic.h>

#define _MAX_ADDRESS 0xFFFF
#define _MIN_CHUNK_SIZE = 2;

typedef struct chunk_meta {
    size_t size;
    void *freeptr;
} chunk_meta;

void *free_ptr_head = 0;
void *bottom_ptr = 0;

#define GET_CHUNK_META(ptr) (chunk_meta *) ((ptr) - sizeof(chunk_meta))


void *malloc(size_t size) {
    chunk_meta chunk;
    void *ptr = 0;
    chunk_meta prev_chunk;

    while (1) {
        chunk = GET_CHUNK_META(ptr);
        if (!chunk.freeptr) break;

        prev_chunk = 

        int can_split_chunk = size > (chunk.size + sizeof(chunk_meta))
        if (size == chunk.size  // is the chunk exactly the right size?
            || can_split_chunk) { // or are we able to split the chunk?
            chunk.size = size;

            // remove from free list
            *prev_freeptr = chunk.freeptr;
            chunk.freeptr = 0;
        }
        ptr = chunk.freeptr;
    }

    // if we get here, nothing interesting in the free list. just create new chunk
    ptr = bottom_ptr - size;
    chunk = GET_CHUNK_META(ptr);

    bottom_ptr = ptr - sizeof(chunk_meta);
    if (bottom_ptr <= __END__) {
        // we hit the binary base!
        panic("Heap hit the binary base!");
    }

    chunk.size = size;
    chunk.freeptr = 0; // chunk in use

    return ptr;
}
