#include <errno.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>

#if defined(__CORE_DEBUG)
#include "usart.h"
#endif

extern char __HeapLimit;
extern char __StackLimit;

void
DDL_AssertHandler(const char* file, int line) {
    (void)file;
    (void)line;
    while (1) {}
}

int
_close(int file) {
    (void)file;
    return -1;
}

int
_fstat(int file, struct stat* st) {
    (void)file;
    st->st_mode = S_IFCHR;
    return 0;
}

int
_isatty(int file) {
    (void)file;
    return 1;
}

int
_getpid(void) {
    return 1;
}

int
_kill(int pid, int sig) {
    (void)pid;
    (void)sig;
    errno = EINVAL;
    return -1;
}

int
_lseek(int file, int ptr, int dir) {
    (void)file;
    (void)ptr;
    (void)dir;
    return 0;
}

int
_read(int file, char* ptr, int len) {
    (void)file;
    (void)ptr;
    (void)len;
    return 0;
}

caddr_t
_sbrk(int incr) {
    static char* heap_end;
    char* prev_heap_end;

    if (heap_end == 0) {
        heap_end = &__HeapLimit;
    }

    prev_heap_end = heap_end;
    if (heap_end + incr > &__StackLimit) {
        errno = ENOMEM;
        return (caddr_t)-1;
    }

    heap_end += incr;
    return (caddr_t)prev_heap_end;
}

int
_write(int file, char* ptr, int len) {
    (void)file;
#if defined(__CORE_DEBUG)
    usart_write_buffer((const uint8_t*)ptr, (uint32_t)len);
#else
    (void)ptr;
#endif
    return len;
}
