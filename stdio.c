#include <stdio.h>

inline char getc() {
    char c;
    asm in 0 c;
    return c;
}

inline int putc(const char c) {
    asm out c 0;
    return (size_t) 1;
}

size_t read(char *buf, size_t n) {
    for (int i = 0; i < n; i++) {
        *buf = getc();
        buf++;
    }
    return n;
}

// dangerous obviously
size_t gets(char *s) {
    char *orig_s = s;
    
    while (*s != '\n' && *s != '\0') {
        *s = getc();
        s++;
    }

    return (size_t) (orig_s - s);
}

size_t puts(char *s) {
    char *orig_s = s;
    if (s) {
        while (*s) {
            putc(*s);
            s++;
        }
    }
    return (size_t) (orig_s - s);
}

