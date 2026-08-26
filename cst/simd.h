#pragma once
#include "type.h"

namespace cst::simd {
    typedef float f32x4 __attribute__((__vector_size__(16))); 
    typedef float f32x2 __attribute__((__vector_size__(8)));  
    typedef i32   i32x4 __attribute__((__vector_size__(16))); 
    typedef u8    u8x16 __attribute__((__vector_size__(16)));

    typedef i64   i64x2 __attribute__((__vector_size__(16)));
    typedef i32   i32x8 __attribute__((__vector_size__(32)));
    
    #if defined(__clang__)
    //typedef float f32_mat4x4 __attribute__((__matrix_type__(4, 4)));
    #endif
}
