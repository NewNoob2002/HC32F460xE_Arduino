#include "lvgl.h"

typedef void *(*alloc_func_t)(size_t);

static void *first_alloc(size_t size);

static alloc_func_t alloc_func = first_alloc;

static void *first_alloc(const size_t size)
{
    if (!lv_is_initialized()) {
        lv_init();
    }

    alloc_func = lv_malloc;
    return lv_malloc(size);
    ;
}

void *operator new(size_t size)
{
    return alloc_func(size);
}

void *operator new[](size_t size)
{
    return alloc_func(size);
}

void operator delete(void *ptr)
{
    lv_free(ptr);
}

void operator delete[](void *ptr)
{
    lv_free(ptr);
}

//__asm(".global __use_no_heap_region\n\t");
void *malloc(size_t size)
{
    return alloc_func(size);
}

void free(void *p)
{
    lv_free(p);
}

void *realloc(void *p, size_t want)
{
    return lv_realloc(p, want);
}

void *calloc(size_t nmemb, size_t size)
{
    return lv_calloc(nmemb, size);
}
