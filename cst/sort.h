#pragma once

#include "type.h"

namespace cst {

    enum class SORT_STYLE : u8 {
        ASCENDING,
        DESCENDING
    };
    
    void string_sort(void *begin, void *end, SORT_STYLE style);
    void number_sort(f64 *begin, f64 *end, SORT_STYLE style);
    
}