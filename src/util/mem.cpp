#include "mem_util.h"
#include "simd.h"

void cst::memset(void *dest, const u8 copy, const dint size) {
    u8 *source_dest = static_cast<u8*>(dest);
    for (dint i = 0; i < size; i++) {
        source_dest[i] = copy; 
    }
}

void cst::memcpy(void *dest, const void *copy, const dint size) {
    u8 *source_dest = static_cast<u8*>(dest);
    const u8 *source_copy = static_cast<const u8*>(copy);
    for (dint i = 0; i < size; i++) {
        source_dest[i] = source_copy[i]; 
    }
}

void cst::memmove(void *dest, const void *src, const dint size) {
    if (!dest || !src || size <= 0) return;

    u8 *byte_dest = static_cast<u8*>(dest);
    const u8 *byte_src = static_cast<const u8*>(src);

    if (byte_dest < byte_src || byte_dest >= byte_src + size) {
        for (dint i = 0; i < size; i++) {
            byte_dest[i] = byte_src[i];
        }
    } 

    else {
        for (dint i = size - 1; i >= 0; i--) {
            byte_dest[i] = byte_src[i];
        }
    }
}

bool cst::memeq(const void *cmp1, const void *cmp2, const dint size) {
    const u8* p1 = static_cast<const u8*>(cmp1);
    const u8* p2 = static_cast<const u8*>(cmp2);

    dint n = size;
    while (n >= 16) {
        simd::u8x16 v1, v2;
        __builtin_memcpy(&v1, p1, 16);
        __builtin_memcpy(&v2, p2, 16);

        simd::u8x16 diff = (cst::simd::u8x16)(v1 != v2);
        simd::i64x2 mask = (simd::i64x2)diff;

        if (mask[0] | mask[1]) return false; 

        p1 += 16; p2 += 16; n -= 16;
    }

    if (n >= 8) {
        u64 v1, v2;
        __builtin_memcpy(&v1, p1, 8);
        __builtin_memcpy(&v2, p2, 8);
        if (v1 != v2) return false;
        p1 += 8; p2 += 8; n -= 8;
    }

    if (n >= 4) {
        u32 v1, v2;
        __builtin_memcpy(&v1, p1, 4);
        __builtin_memcpy(&v2, p2, 4);
        if (v1 != v2) return false;
        p1 += 4; p2 += 4; n -= 4;
    }

    while (n--) {
        if (*p1++ != *p2++) return false;
    }

    return true;
}
 
bool cst::memcmp(const void *cmp1, const void *cmp2, const dint size) {
    const u8* p1 = static_cast<const u8*>(cmp1);
    const u8* p2 = static_cast<const u8*>(cmp2);

    dint n = size;

    while (n >= 16) {
        simd::u8x16 v1, v2;
        __builtin_memcpy(&v1, p1, 16);
        __builtin_memcpy(&v2, p2, 16);

        simd::u8x16 diff = (cst::simd::u8x16)(v1 != v2);
        simd::i64x2 mask = (simd::i64x2)diff;

        if (mask[0] | mask[1]) {
            for (int i = 0; i < 16; ++i) {
                if (p1[i] != p2[i]) {
                    return static_cast<int>(p1[i]) - static_cast<int>(p2[i]);
                }
            }
        }
        p1 += 16; p2 += 16; n -= 16;
    }
    
    if (n >= 8) {
        u64 v1, v2;
        __builtin_memcpy(&v1, p1, 8);
        __builtin_memcpy(&v2, p2, 8);
        if (v1 != v2) {

            v1 = __builtin_bswap64(v1);
            v2 = __builtin_bswap64(v2);
            return (v1 > v2) ? 1 : -1;
        }
        p1 += 8; p2 += 8; n -= 8;
    }

    while (n--) {
        if (*p1 != *p2) {
            return static_cast<int>(*p1) - static_cast<int>(*p2);
        }
        ++p1; ++p2;
    }

    return 0;
}