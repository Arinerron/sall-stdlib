#include "malloc.h"


void hexdump(void *ptr, size_t n) {
    // TODO
}


void print_chunk(chunk_meta *chunk) {
    print("[ CHUNK 0x");
    print_hex((uint16_t)chunk);
    print(" ]\n");

    print("State: ");
    print((IS_CHUNK_FREE(chunk) ? "FREE" : "IN-USE"));
    print("\nFree Pointer: 0x");
    print_hex((uint16_t)(chunk->freeptr));
    print("\n");
}


void print_heap() {
    print("========== BEGIN HEAP DUMP ==========\n\n");

    size_t total_inuse, size_t total_free;

    void *ptr = bottom_ptr;
    while (ptr != _MAX_ADDRESS) {
        chunk_meta *chunk = GET_CHUNK_META(ptr);
        if (IS_CHUNK_FREE(chunk)) {
            total_free += chunk->size;
        } else {
            total_inuse += chunk->size;
        }

        print_chunk(chunk);

        ptr = chunk->freeptr;
        print("\n");
    }

    print("=====================================\n");
    
    print("Total in-use bytes: 0x");
    print_hex(total_inuse);
    print("\nTotal free bytes: 0x");
    print_hex(total_free);
    print("\nTotal bytes used by the heap: 0x");
    print_hex((int)(_MAX_ADDRESS - bottom_ptr)); // the difference is because of sizeof(chunk_meta)s

    print("\n=========== END HEAP DUMP ===========\n");
}
