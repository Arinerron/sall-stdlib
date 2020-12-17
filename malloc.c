#include "stdlib.h"
#include "string.h"
#include "malloc.h"


void *malloc(size_t size) {
    ASSERT(size < _MAX_ADDRESS, "Cannot allocate above size _MAX_ADDRESS");
    if (!size) return NULL;

    // first, iterate over the free list and see if we have a chunk that'll fit
    chunk_meta *chunk;
    void *ptr = free_ptr_head;
    void **prev_freeptr = &free_ptr_head;

    while (ptr) {
        chunk = GET_CHUNK_META(ptr);
        if (!IS_CHUNK_FREE(chunk)) break; // chunk list is terminated by a null byte (which also happens to signify in use chunks)

        int can_split_chunk = size > (chunk->size + sizeof(chunk_meta));
        if (can_split_chunk) {
            chunk_meta *new_chunk = (chunk_meta *)(ptr + size);
            new_chunk->size = chunk->size - (sizeof(chunk_meta) + size);

            // insert new, split chunk into linked list
            new_chunk->freeptr = *prev_freeptr;
            *prev_freeptr = ((void *)new_chunk) + sizeof(chunk_meta);
        }

        if (size == chunk->size || can_split_chunk) {
            chunk->size = size;

            // remove from free list
            *prev_freeptr = chunk->freeptr;
            chunk->freeptr = 0;

            return ptr;
        }

        prev_freeptr = &(chunk->freeptr);
        ptr = chunk->freeptr;
    }

    // if we get here, nothing interesting in the free list. just create new chunk
    ptr = bottom_ptr - size;
    chunk = GET_CHUNK_META(ptr);

    bottom_ptr = ptr - sizeof(chunk_meta);
    ASSERT(bottom_ptr > __END__, "Out of memory; heap bottom pointer is below the binary end address");

    chunk->size = size;
    chunk->freeptr = 0; // chunk in use

    return ptr;
}


void free(void *ptr) {
    ASSERT(ptr >= __END__, "Cannot free chunk that is inside of the binary end address");
    if (!ptr) return;

    chunk_meta *chunk = GET_CHUNK_META(ptr);
    ASSERT(chunk->size, "Attempting to free chunk of NULL size");
    ASSERT(!chunk->freeptr, "Attempting to double free chunk (!chunk->freeptr)");
    ASSERT(ptr != free_ptr_head, "Attempting to double free a chunk (ptr == free_ptr_head)");

    chunk->freeptr = free_ptr_head;
    free_ptr_head = chunk->freeptr;

    // check if we can consolidate with the chunk in front of the current one
    void *next_ptr = ptr + chunk->size + sizeof(chunk_meta);
    chunk_meta *next_chunk = GET_CHUNK_META(next_ptr);
    if (IS_CHUNK_FREE(next_chunk)) {
        // yes, we can consolidate. add the size of the next chunk to the cur chunk
        chunk->size += sizeof(chunk_meta) + next_chunk->size;

        // remove the next_chunk from the linked list of free chunks
        chunk_meta *_chunk;
        int found_chunk = 0;
        for (void *_ptr = free_ptr_head; _ptr; _ptr = _chunk->freeptr) { // "&& _chunk" to check?
            if (_ptr) {
                _chunk = GET_CHUNK_META(_ptr);
                if (next_ptr == _chunk->freeptr) {
                    found_chunk = 1;
                    _chunk->freeptr = next_chunk->freeptr;
                    break;
                }
            }
        }

        ASSERT(found_chunk, "Failed to find chunk in free chunks linked list");
    }
}


void *realloc(void *ptr, size_t size) {
    // basic simplification checks
    if (!size) {
        free(ptr);
        return NULL;
    } else if (!ptr) {
        return malloc(size);
    }

    chunk_meta *chunk = GET_CHUNK_META(ptr);
    ASSERT(!IS_CHUNK_FREE(chunk), "Reallocing a freed chunk");
    if (chunk->size == size) {
        // NOTE: this if case is unnecessary, but is a slight optimization
        return chunk;
    }

    // check if requesting to shrink the chunk
    if (size < chunk->size && chunk->size - size > sizeof(chunk_meta)) {
        // create a fake chunk then free it
        void *next_ptr = ptr + chunk->size + sizeof(chunk_meta);
        chunk_meta *next_chunk = GET_CHUNK_META(ptr);

        next_chunk->size = chunk->size - size - sizeof(chunk_meta);
        next_chunk->freeptr = 0;
        free(next_ptr);

        chunk->size = size;
        return ptr;
    }
    // check if requesting to grow the chunk
    else if (size > chunk->size) {
        // malloc new chunk, memcpy stuff, then free old
        void *new_ptr = malloc(size);
        memcpy(new_ptr, ptr, size);
        free(ptr);
        return new_ptr;
    } else {
        chunk->size = size;
        return ptr;
    }
}

