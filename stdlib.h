#include "types.h"
#include "stdio.h"

void panic(const char *msg) {
    puts("PANIC: ");
    puts(msg);
    putc('\n');
    exit(); // TODO
}

#define ASSERT(statement, msg) if (!(statement)) { panic((msg)); }
