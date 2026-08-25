#pragma once

#include "type.h"

namespace cst {
    void memset(void *dest, const u8 copy, const dint size);
    void memcpy(void *dest, const void *copy, const dint size);
    void memmove(void *dest, const void *src, const dint size);
    bool memcmp(const void *cmp1, const void *cmp2, const dint size);
    bool memeq(const void *cmp1, const void *cmp2, const dint size);
}