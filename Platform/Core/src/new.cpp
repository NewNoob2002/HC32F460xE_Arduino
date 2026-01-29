#include "lwmem/lwmem.h"

#ifdef LWMEM_HDR_H
#define pool_size        32 * 1024
uint8_t malloc_memory_area[pool_size] = {};
static lwmem_region_t regions[]       = {{malloc_memory_area, sizeof(malloc_memory_area)},
                                         {NULL, 0}};
#endif

static uint8_t is_initialized            = 0;
																				 
typedef void *(*alloc_func_t)(size_t);

static void *first_alloc(size_t size);

static alloc_func_t alloc_func = first_alloc;

static void *first_alloc(const size_t size)
{
#ifdef LWMEM_HDR_H
    if (!is_initialized) {
        if (lwmem_assignmem(regions)) {
					is_initialized = 1;
				}
				else{
					return nullptr;
				}
    }

    alloc_func = lwmem_malloc;
    return lwmem_malloc(size);
#endif
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
#ifdef LWMEM_HDR_H
    lwmem_free(ptr);
#endif
}

void operator delete[](void *ptr)
{
#ifdef LWMEM_HDR_H
    lwmem_free(ptr);
#endif
}

//__asm(".global __use_no_heap_region\n\t");
void *malloc(size_t size)
{
    return alloc_func(size);
}

void free(void *p)
{
#ifdef LWMEM_HDR_H
    lwmem_free(p);
#endif
}

void *realloc(void *p, size_t want)
{
#ifdef LWMEM_HDR_H
    return lwmem_realloc(p, want);
#endif
}

void *calloc(size_t nmemb, size_t size)
{
#ifdef LWMEM_HDR_H
    return lwmem_calloc(nmemb, size);
#endif
}
