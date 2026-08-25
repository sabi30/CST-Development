#include "macro.h"
#include "mem_util.h"

#include "mem.h"
#include "io.h"

const u32 BLOCK_SIZE = 65536;
const u32 PAGE_COUNT = 64;

struct block_s {  
    block_s *next;
};

struct page_s {
    block_s *next;
    u32 size;
    u32 used;
    u8 payload[BLOCK_SIZE - 64];
};

namespace {
    page_s *page_list[PAGE_COUNT];
}

void *cst::malloc(dint size) {
    if (!size) return nullptr;

    dint alloc_size = (size + 15) & ~15;
    dint alloc_class = alloc_size / 16;

    page_s *curr_page = page_list[alloc_class];
    
}

void *cst::calloc(dint type_size, dint size) {
    void *ptr = malloc(type_size * size);
    u8* byte_list = reinterpret_cast<u8*>(ptr);

    for (u32 i = 0; i < size; i++) {
        u32 offset = i * type_size;
        byte_list[offset] = 0;
    }
    
    return ptr;
}

void *cst::crealloc(void* ptr, dint size) {

}

void cst::cfree(void *ptr) {

}

// PUBLIC

namespace cst {
    void* (*alloc)(dint) = malloc;
    void (*free)(void*) = cfree;
    void *(*realloc)(void*, dint) = crealloc;
}
