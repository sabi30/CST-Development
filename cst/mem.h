#pragma once

#include "func.h"
#include "type.h"

namespace cst {
    enum class PROT_FLAGS : u8 {
        NONE    = 0,
        READ    = 1 << 0,
        WRITE   = 1 << 1,
        EXEC    = 1 << 2
    };

    enum class MMAP_RESULT : u8 {
        SUCCESS,
        ERROR,

        // modify //
        ERROR_MODIFY_FLAG_COMBINATION,
    };

    void *malloc(dint size);                                // OS-ALLOC
    void *calloc(dint type_size, dint size);                // OS-ALLOC
    void *crealloc(void*, dint);                            // OS-ALLOC
    void cfree(void*);                                      // OS-ALLOC
    
    extern void* (*alloc)(dint);                 // MAIN ALLOC
    extern void (*free)(void*);                  // MAIN FREE
    extern void *(*realloc)(void*, dint);        // MAIN REALLOC

    struct memoryinfo_t {
        bool read;
        bool write;
        bool execute;
    };
    
    void *mmap(dint size, u8 prot_flags = (u8)PROT_FLAGS::READ | (u8)PROT_FLAGS::WRITE);
    MMAP_RESULT unmap(void *map, dint size);
    MMAP_RESULT mmodify(void *ptr, dint size, u8 prot_flags);

    namespace memory_profiler {
        extern dint PEAK_MEMORY_USAGE;
        extern dint AVG_MEMORY_USAGE;
        extern dint MEMORY_USAGE;

        void allocated(dint size);
        void freed(dint size);
    }
}