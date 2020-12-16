#include "stdio.h"

void panic(const char *msg) {
    puts("Panic: ");
    puts(msg);
    exit(); // TODO
}
