#include "macro.h"
#include "mem_util.h"

#include "mem.h"
#include "io.h"

#ifdef CST_USE_LIBC
#include <stdlib.h>
#endif

// RUNTIME SECURITY //
void *empty_user_alloc(dint) {
    cst::print("[CST]: warning: 'void * alloc (dint)' has been called without an allocator!");
    return nullptr;
}

void empty_user_free(void*) {
    cst::print("[CST]: warning: 'void free (void*)' has been called without an deallocator!");
}

void *empty_user_realloc(void*, dint) {
    cst::print("[CST]: warning: 'void * realloc (dint)' has been called without an reallocator!");
    return nullptr;
}
// END RUNTIME SECURITY //

#ifdef CST_USE_LIBC
static void *c_library_malloc(dint size) {
    return malloc(size);
}

static void c_library_free(void *ptr) {
    return free(ptr);
}

static void *c_library_realloc(void *ptr, dint new_size) {
    return realloc(ptr, new_size);
}
#endif

struct alignas(16) memory_header {
    dint size;
};

namespace cst {
#ifdef CST_USE_LIBC
    void* (*alloc)(dint) = malloc;
    void (*free)(void*) = cfree;
    void *(*realloc)(void*, dint) = crealloc;
#else
    void* (*alloc)(dint) = empty_user_alloc;
    void (*free)(void*) = empty_user_free;
    void *(*realloc)(void*, dint) = empty_user_realloc;
#endif
}

void *cst::malloc(dint size) {
#ifdef CST_USE_LIBC
    cst::memory_profiler::allocated(size);
    dint *ptr = static_cast<dint*>(c_library_malloc(size + sizeof(dint)));
    *ptr = size;
    return ptr + 1;
#endif
}

void *cst::calloc(dint type_size, dint size) {
    void *ptr = malloc(type_size * size);
    u8* byte_list = reinterpret_cast<u8*>(ptr);

    for (u32 i = 0; i < size; i++) {
        u32 offset = i * type_size;
        byte_list[offset] = 0;
    }
    
    return malloc(type_size * size);
}

void *cst::crealloc(void* ptr, dint size) {
#ifdef CST_USE_LIBC 
    if (!ptr) {
        return malloc(size);
    }
    dint *old_header = static_cast<dint*>(ptr) - 1;
    dint old_size = *old_header;

    dint *new_header = static_cast<dint*>(
        c_library_realloc(old_header, size + sizeof(dint))
    );
    if (!new_header) return nullptr;

    *new_header = size;

    cst::memory_profiler::allocated(size - old_size);

    return new_header + 1;
#endif
}

void cst::cfree(void *ptr) {
#ifdef CST_USE_LIBC 
    dint *size_header = static_cast<dint*>(ptr) - 1;
    cst::memory_profiler::freed(*size_header);
    c_library_free(size_header);
#endif
}

