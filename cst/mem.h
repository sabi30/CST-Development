#pragma once

#include "func.h"
#include "type.h"

namespace cst {
    const u32 PROT_NONE = 0;
    const u32 PROT_READ = 1 << 0;
    const u32 PROT_WRITE = 1 << 1;
    const u32 PROT_EXEC = 1 << 2;

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
    
    void *mmap(dint size, u8 prot_flags = PROT_READ | PROT_WRITE);
    bool unmap(void *map, dint size);
    bool mmodify(void *ptr, dint size, u8 prot_flags);

    namespace memory_profiler {
        extern dint PEAK_MEMORY_USAGE;
        extern dint AVG_MEMORY_USAGE;
        extern dint MEMORY_USAGE;

        void allocated(dint size);
        void freed(dint size);
    }
}