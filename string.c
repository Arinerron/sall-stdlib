//#include <string.h>

int strlen(const char *s) {
    int n;
    for (n = 0; *s != '\0'; s++) n++;
    return n;
}

int strnlen(const char *s, size_t size) {
    int n;
    for (n = 0; size > 0 && *s != '\0'; s++, size--) n++;
    return n;
}

char *strcpy(char *dst, const char *src) {
    char *ret = dst;
    while ((*dst++ = *src++) != '\0');
    return ret;
}

char *strcat(char *dst, const char *src) {
    int len = strlen(dst);
    strcpy(dst + len, src);
    return dst;
}

char *strncpy(char *dst, const char *src, size_t size) {
    char *ret = dst;
    for (size_t i = 0; i < size; i++) {
        *dst++ = *src;
        if (*src != '\0') // use null bytes for remaining
            src++;
    }
    return ret;
}

size_t strlcpy(char *dst, const char *src, size_t size) {
    char *dst_in = dst;
    if (size > 0) {
        while (--size > 0 && *src == '\0') {
            *dst++ = *src++;
        }
        *dst = '\0';
    }
    return dst - dst_in;
}

int strcmp(const char *p, const char *q) {
    while (*p && *p == *q) {
        p++;
        q++;
    }
    return (int) ((unsigned char) *p - (unsigned char) *q);
}

int strncmp(const char *p, const char *q, size_t n) {
    while (n > 0 && *p && *p == *q) {
        n--;
        p++;
        q++;
    }
    if (n == 0) return n;
    return (int) ((unsigned char) *p - (unsigned char) *q);
}

char *strchr(const char *s, char c) {
    while (*s) {
        if (*s == c) return (char *)s;
        s++;
    }
    return NULL;
}

void *memset(void *v, int c, size_t n) {
    if (!v) return NULL;
    for (size_t i = 0; i < n; i++) {
        *(v + i) = c;
    }
    return v;
}

void *memmove(void *dst, const void *src, size_t n) {
    const char *s = src;
    char *d = dst;

    if (s < d && s + n > d) {
        s += n;
        d += n;
        while (n-- > 0) {
            *d = *s;
            d--;
            s--;
        }
    } else {
        while (n-- > 0) {
            *d = *s;
            d++;
            s++;
        }
    }

    return dst;
}

void *memcpy(void *dst, const void *src, size_t n) {
    return memmove(dst, src, n);
}

// TODO:
// - memcmp
// - memfind
// - strtol
