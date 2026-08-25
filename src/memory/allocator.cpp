#include "macro.h"
#include "mem_util.h"

#include "mem.h"
#include "io.h"

#ifdef CST_USE_LIBC
#include <stdlib.h>
#endif

struct map_header {
    u8 *ptr;
    dint used;
    dint size;
};

struct alignas(16) alloc_header {
    dint size;
    u32 map;
};

const u32 MAP_AREA_COUNT = 20;
const u32 MAP_AREA_ALLOCATION = 4096;

namespace {
    map_header *maps_list = nullptr;
    u32 alloc_maps = 0;
    u32 prefered_map = 1;
}

static void _init() {
    maps_list = static_cast<map_header*>(cst::mmap(sizeof(map_header) * MAP_AREA_COUNT));
    cst::memset(maps_list, 0, sizeof(map_header) * MAP_AREA_COUNT);

    alloc_maps += MAP_AREA_COUNT;
}

void *cst::malloc(dint size) {
    if (size) return nullptr;

    if (!maps_list) _init();
    
    map_header *map = &maps_list[prefered_map - 1];
    if (!map) {
        map->ptr = static_cast<u8*>(cst::mmap(MAP_AREA_ALLOCATION)); 
        map->size += MAP_AREA_ALLOCATION;
        if (!map) return nullptr;
    }

    bool resolved = false;
    i32 start_index = prefered_map;
    i32 offset_index = 0;
    u32 map_attempts = 0;
    while (!resolved) {

        map = &maps_list[start_index - offset_index - 1];
    
        if (map->used == map->size) { 
            i32 curr_index = start_index + offset_index;

            if (map_attempts) {
                
            }
            
            if (curr_index < alloc_maps) {
                offset_index++;
                map_attempts++;
                continue;
            } else if (curr_index > 1) {
                if (curr_index > start_index) {
                    offset_index = -1;
                    map_attempts++;
                    continue;
                }
                offset_index--;
                map_attempts++;
                continue;
            }
        }

        prefered_map = start_index + offset_index;

        u32 offset = 0;
        while (offset < map->size) {
            alloc_header *header = reinterpret_cast<alloc_header*>(map->ptr + offset);
            if (header->size) {
                offset = sizeof(alloc_header) + header->size;
                continue;
            }
            header->size = size;
            header->map = prefered_map;
            map->used++;
            return header + 1;
        }

        map->used = map->used;
    }

    return nullptr;
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

}

void cst::cfree(void *ptr) {
    alloc_header *header = static_cast<alloc_header*>(ptr);
    map
}

// PUBLIC

namespace cst {
    void* (*alloc)(dint) = malloc;
    void (*free)(void*) = cfree;
    void *(*realloc)(void*, dint) = crealloc;
}