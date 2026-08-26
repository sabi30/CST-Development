#include "macro.h"
#include "mem_util.h"

#include "mem.h"
#include "io.h"

// REWRITE

static const u32 PAYLOAD_SIZE = 65536;

static const u32 SIZE_CLASSES[64] = {  
    2, 4, 8, 16, 32, 64,

    80, 96, 112, 128,
    160, 192, 224, 256,
    320, 384, 448, 512,
    640, 768, 896, 1024,

    1152, 1280, 1408, 1536, 1664, 1792, 1920, 2048,  
    2304, 2560, 2816, 3072, 3328, 3584, 3840, 4096,  
    4608, 5120, 5632, 6144, 6656, 7168, 7680, 8192,  
    
    9216, 10240, 11264, 12288, 13312, 14336, 15360, 16384,

    18432, 20480, 22528, 24576, 28672, 32768,

    40960, 49152, 57344, PAYLOAD_SIZE
};

static u32 get_class(u64 size) {
    if (size > PAYLOAD_SIZE) return 65; 

    u32 idx = 0;
    idx += (SIZE_CLASSES[idx + 31] < size) ? 32 : 0;
    idx += (SIZE_CLASSES[idx + 15] < size) ? 16 : 0;
    idx += (SIZE_CLASSES[idx + 7]  < size) ? 8  : 0;
    idx += (SIZE_CLASSES[idx + 3]  < size) ? 4  : 0;
    idx += (SIZE_CLASSES[idx + 1]  < size) ? 2  : 0;
    idx += (SIZE_CLASSES[idx + 0]  < size) ? 1  : 0;

    return idx;
}

struct sblock {
    sblock *next;
    u32 size_class;
};

struct spage {
    sblock *free_list = nullptr;
    u32 size = 0;
    u32 used = 0;
    u8 payload[PAYLOAD_SIZE];

    u64 dynamic_size = 0;
    u64 dynamic_used = 0;
    u8 *dynamic_payload = nullptr;
};

namespace {
    void *mmap_ptrs[32];
    spage *pages[64];
}

static void _create_page(dint size_class) {
    spage *page = static_cast<spage*>(cst::mmap(sizeof(spage)));
    *page = {};
    page->size = SIZE_CLASSES[size_class - 1];
    
    pages[size_class] = page;

    sblock *prev_block = nullptr;

    dint i = 0;
    while (i < PAYLOAD_SIZE) {
        sblock *block = reinterpret_cast<sblock*>(&page->payload[i]);
        block->size_class = size_class;
        if (prev_block) {
            prev_block->next = block;
        }
        prev_block = block;

        i += sizeof(sblock) + SIZE_CLASSES[size_class];
    }
    sblock *block = reinterpret_cast<sblock*>(&page->payload[0]);
    page->free_list = block;
}

void *cst::malloc(dint size) {
    if (!size) return nullptr;


    u32 size_class = get_class(size);
    if (size_class > 64) {
        void *memory_map = mmap(size);
        for (dint i = 0; i < 32; i++) {
            if (mmap_ptrs[i]) continue;
            mmap_ptrs[i] = memory_map;

            memory_profiler::allocated(size);
            return memory_map;
        }  
        return nullptr;
    }

    spage *page = pages[size_class];
    while (true) {
        if (!page) {
            _create_page(size_class);
            page = pages[size_class];
            sblock *block = page->free_list;
            page->free_list = block->next;
            return block + 1;
        }

        if (!page->free_list) {
            // dynamic payload later
            return nullptr;
        }

        sblock *block = page->free_list;
        page->free_list = block->next;
        return block + 1;
    }

    return nullptr;
}

void *cst::crealloc(void *ptr, dint new_size) {
    if (!new_size) return nullptr;
    if (!ptr) return malloc(new_size);
    
    sblock *block = reinterpret_cast<sblock*>(ptr) - 1;

    sblock *new_block = reinterpret_cast<sblock*>(malloc(new_size)) - 1;

    dint size_to_copy = new_size;
    if (block->size_class < new_size) size_to_copy = block->size_class;
    
    memcpy(new_block + 1, block + 1, new_size);
    return new_block + 1; 
}

void cst::cfree(void *ptr) {
    sblock *block = reinterpret_cast<sblock*>(ptr) - 1;
    block->next = pages[block->size_class]->free_list;
    pages[block->size_class]->free_list = block;
}

// PUBLIC

namespace cst {
    void* (*alloc)(dint) = malloc;
    void (*free)(void*) = cfree;
    void *(*realloc)(void*, dint) = crealloc;
}


/* const u32 BLOCK_SIZE = 65536;
const u32 PAGE_COUNT = 64;

const u32 CLASS_TO_SIZE[64] = {
// SMALL TIER
2, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128,
// MEDIUM TIER
160, 192, 224, 256, 320, 384, 448, 512, 576, 640, 704, 768, 832, 896, 960, 1024,
// LARGE TIER
1280, 1536, 1792, 2048,
2560, 3072, 3584, 4096,
5120, 6144, 7168, 8192,
10240, 12288, 14336, 16384,
20480, 24576, 28672, 32768,
40960, 49152, 57344, 65536,
81920, 98304, 114688, 131072,
163840, 196608, 229376
};

static u32 size_to_class(dint size) {
    if (size <= 128) {
        return static_cast<u32>((size + 7) >> 3) - 1;
    }

    u64 s = static_cast<u64>(size - 1);

    u32 highest_bit = 63 - __builtin_clzll(s);

    u32 shift = highest_bit - 2;
    u32 sub_bin = (s >> shift) & 0x3;

    u32 bin = (highest_bit - 7) * 4 + sub_bin + 16;
    return bin < 64 ? bin : 63;
}

struct block_s {
    block_s *next;
    u8 located_class;
};

struct page_s {
    block_s *free_list;
    u32 size;
    u32 used;
    u8 payload[BLOCK_SIZE - 64];
};

namespace {
    page_s *page_list[PAGE_COUNT];
}

void *cst::malloc(dint size) {
    if (!size) return nullptr;

    dint alloc_class = size_to_class(size);

    page_s *page = page_list[alloc_class];

    if (!page || !page->free_list) {
        page = static_cast<page_s*>(mmap(BLOCK_SIZE));
        page->size = CLASS_TO_SIZE[alloc_class];
        page->used = 0;

        block_s *prev_block = reinterpret_cast<block_s*>(&page->payload);

        // first block
        page->free_list = prev_block;

        dint i = page->size;
        while (i < BLOCK_SIZE) {
            block_s *block = reinterpret_cast<block_s*>(&page->payload[i]);
            block->located_class = alloc_class;
            block->next = nullptr;

            prev_block->next = block;
            prev_block->located_class = alloc_class;

            if (!page->free_list) {
                page->free_list = block;
            } else {
                prev_block->next = block;
            }

            i += (page->size <= (BLOCK_SIZE - sizeof(page_s)));
        }

        page_list[alloc_class] = page;
    }

    block_s *block = page_list[alloc_class]->free_list;
    page_list[alloc_class]->free_list = block->next;
    page_list[alloc_class]->used++;

    return block + 1;
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
    if (!ptr) return malloc(size);
    void *new_alloc = malloc(size);
    block_s *block = static_cast<block_s*>(ptr) - 1;
    dint class_size = CLASS_TO_SIZE[block->located_class];

    if (class_size < size) {
        memcpy(new_alloc, ptr, class_size);
    } else {
        memcpy(new_alloc, ptr, size);
    }

    return new_alloc;
}

void cst::cfree(void *ptr) {
    block_s *block = static_cast<block_s*>(ptr) - 1;
    print("%p, %u\n", block->next, block->located_class);
    page_list[block->located_class]->used--;
    block->next = page_list[block->located_class]->free_list;
    page_list[block->located_class]->free_list = block;
} */
