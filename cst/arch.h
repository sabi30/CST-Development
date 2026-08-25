#pragma once

namespace cst::arch {
    #if defined(__x86_64__) || defined(_M_X64) || defined(__i386__)
    typedef float __m128_raw __attribute__((__vector_size__(16), __aligned__(16)));
    typedef double __m128d_raw __attribute__((__vector_size__(16), __aligned__(16)));
    typedef long long __m128i_raw __attribute__((__vector_size__(16), __aligned__(16)));
    
    #ifdef __AVX__
    typedef float __m256_raw __attribute__((__vector_size__(32), __aligned__(32)));
    #endif
    #ifdef __AVX512F__
    typedef float __m512_raw __attribute__((__vector_size__(64), __aligned__(64)));
    #endif
#elif defined(__arm__) || defined(__aarch64__) || defined(_M_ARM64)
    typedef __attribute__((__neon_vector_type__(4))) float float32x4_raw;
    typedef __attribute__((__neon_vector_type__(2))) long long int64x2_raw;
    // typedef __attribute__((__neon_vector_type__(16))) char int8x16_raw;
    
    #ifdef __ARM_FEATURE_SVE
    using sve_f32 = __SVFloat32_t;
    using sve_i32 = __SVInt32_t;
    #endif
#endif
}
