#include <errno.h>
#include <new>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "dynamic_memory.h"
#include "lwmem/lwmem.h"

extern "C" {
extern uint8_t __HeapBase;
extern uint8_t __HeapLimit;
}

namespace {

bool memory_initialized = false;
uint8_t heap_buffer[64 * 1024];

bool
initialize_memory() noexcept {
    if (memory_initialized) {
        return true;
    }

    const uintptr_t heap_begin = reinterpret_cast<uintptr_t>(&__HeapBase);
    const uintptr_t heap_end = reinterpret_cast<uintptr_t>(&__HeapLimit);
    if (heap_end <= heap_begin) {
        return false;
    }

    const lwmem_region_t regions[] = {
        {heap_buffer, sizeof(heap_buffer)},
        {reinterpret_cast<void*>(heap_begin), heap_end - heap_begin},
        {nullptr, 0U},
    };
    memory_initialized = lwmem_assignmem(regions) != 0U;
    return memory_initialized;
}

void*
allocate(size_t size) noexcept {
    if (!initialize_memory()) {
        errno = ENOMEM;
        return nullptr;
    }

    const size_t allocation_size = size == 0U ? 1U : size;
    void* const ptr = lwmem_malloc(allocation_size);
    if (ptr == nullptr) {
        errno = ENOMEM;
    }
    return ptr;
}

} // namespace

extern "C" bool
dynamic_memory_init(void) {
    return initialize_memory();
}

extern "C" void*
malloc(size_t size) {
    return allocate(size);
}

extern "C" void*
calloc(size_t count, size_t size) {
    if (size != 0U && count > (SIZE_MAX / size)) {
        errno = ENOMEM;
        return nullptr;
    }
    if (!initialize_memory()) {
        errno = ENOMEM;
        return nullptr;
    }

    void* const ptr = lwmem_calloc(count, size);
    if (ptr == nullptr && count != 0U && size != 0U) {
        errno = ENOMEM;
    }
    return ptr;
}

extern "C" void*
realloc(void* ptr, size_t size) {
    if (!initialize_memory()) {
        errno = ENOMEM;
        return nullptr;
    }

    void* const resized = lwmem_realloc(ptr, size);
    if (resized == nullptr && size != 0U) {
        errno = ENOMEM;
    }
    return resized;
}

extern "C" void
free(void* ptr) {
    if (ptr != nullptr && initialize_memory()) {
        lwmem_free(ptr);
    }
}

void*
operator new(size_t size) {
    return allocate(size);
}

void*
operator new[](size_t size) {
    return allocate(size);
}

void*
operator new(size_t size, const std::nothrow_t&) noexcept {
    return allocate(size);
}

void*
operator new[](size_t size, const std::nothrow_t&) noexcept {
    return allocate(size);
}

void
operator delete(void* ptr) noexcept {
    free(ptr);
}

void
operator delete[](void* ptr) noexcept {
    free(ptr);
}

void
operator delete(void* ptr, size_t) noexcept {
    free(ptr);
}

void
operator delete[](void* ptr, size_t) noexcept {
    free(ptr);
}

void
operator delete(void* ptr, const std::nothrow_t&) noexcept {
    free(ptr);
}

void
operator delete[](void* ptr, const std::nothrow_t&) noexcept {
    free(ptr);
}
