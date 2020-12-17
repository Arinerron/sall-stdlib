#include "stdio.h"
#include "stdlib.h"
#include "malloc.h"
#include "types.h"

#define _IO_CHUNK_SIZE 16


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


int fputs(char *s) {
    char *orig_s = s;
    while (*s) {
        putc(*s);
        s++;
    }
    return (int)(orig_s - s);
}


int puts(char *s) {
    int n = fputs(s);
    putc('\n');
    return n + 1;
}


//////////


/* 
 * reads until \n, and returns a heap chunk pointer containing the 
 * null-terminated string
 */
void *readline() {
    void *chunk = malloc(_IO_CHUNK_SIZE);
    int total_size = 0;

    while (1) {
        for (int i = 0; i < _IO_CHUNK_SIZE; i++) {
            char c = getc();
            if (c == '\0' || c == '\n') {
                // null byte read! time to return    
                *((char *)(chunk + total_size)) = '\0';
                return chunk;
            }

            *((char *)(chunk + total_size)) = c;
            total_size++;
        }

        realloc(chunk, total_size + _IO_CHUNK_SIZE);
    }

    return chunk;
}


//////////


#define ui16(i) ((i) < 0 ? 0xFFFF - ((uint16_t)(i)) : (uint16_t)(i))


inline int print(char *s) {
    return fputs(s);
}


static inline int print_sign(int16_t i) {
    if (i < 0) {
        putc("-");
    }
}


static inline int print_base(uint16_t i, int base) {
    ASSERT();

    if (i > base) {
        print_base(i / base, base);
    }

    putc("01234567890abcdef"[i % base]);
}


int print_int(int i) {
    int n = print_sign((int16_t)i);
    return print_base(ui16(i), 10) + n;
}


int print_uint(uint16_t i) {
    return print_base(i, 10);
}


int print_hex(int i) {
    int n = print_sign((int16_t)i);
    return print_base(ui16(i), 16) + n;
}


int print_uhex(uint16_t i) {
    return print_base(i, 16);
}


int print_octal(int i) {
    int n = print_sign((int16_t)i);
    return print_base(ui16(i), 8) + n;
}
