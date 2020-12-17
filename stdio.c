#include <stdio.h>

#define _IO_CHUNK_SIZE 32


inline char getc() {
    char c;
    asm in 0 c;
    return c;
}


inline int putc(const char c) {
    asm out c 0;
    return (int) 1;
}


int read(char *buf, int n) {
    for (int i = 0; i < n; i++) {
        *buf = getc();
        buf++;
    }
    return n;
}


//////////


// dangerous obviously
int gets(char *s) {
    char *orig_s = s;
    
    while (*s != '\n' && *s != '\0') {
        *s = getc();
        s++;
    }

    *s = '\0';

    return (int) (orig_s - s);
}


int puts(char *s) {
    char *orig_s = s;
    if (s) {
        while (*s) {
            putc(*s);
            s++;
        }
    }
    return (int) (orig_s - s);
}


//////////


/* 
 * reads until \n, and returns a heap chunk pointer containing the 
 * null-terminated string
 */
void *readline() {
    void *chunk = malloc(_IO_CHUNK_SIZE);
    while (1) {
        total_size = 0;
        for (int i = 0; i < _IO_CHUNK_SIZE; i++) {
            char c = getc();
            if (c == '\0' || c == '\n') {
                // null byte read! time to return    
                *(chunk + total_size) = '\0';
                return chunk;
            }

            *(chunk + total_size) = c;
            total_size++;
        }

        realloc(chunk, total_size + _IO_CHUNK_SIZE);
    }

    return chunk;
}


//////////


