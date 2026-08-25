#pragma once

#if defined(__UINT8_TYPE__) && defined(__INT8_TYPE__)
using u8 = __UINT8_TYPE__;      
using i8 = __INT8_TYPE__;
constexpr u8 U8_MAX = __UINT8_MAX__;
constexpr i8 I8_MAX = __INT8_MAX__;
constexpr i8 I8_MIN = -I8_MAX - 1;
constexpr u8 SIZEOF_8 = sizeof(u8);
#endif

#if defined(__UINT16_TYPE__) && defined(__INT16_TYPE__)
using u16 = __UINT16_TYPE__;    
using i16 = __INT16_TYPE__;
constexpr u16 U16_MAX = __UINT16_MAX__;
constexpr i16 I16_MAX = __INT16_MAX__;
constexpr i16 I16_MIN = -I16_MAX - 1;
constexpr u8 SIZEOF_16 = sizeof(u16);
#endif

#if defined(__UINT32_TYPE__) && defined(__INT32_TYPE__)
using u32 = __UINT32_TYPE__;    
using i32 = __INT32_TYPE__;
constexpr u32 U32_MAX = __UINT32_MAX__;
constexpr i32 I32_MAX = __INT32_MAX__;
constexpr i32 I32_MIN = -I32_MAX - 1;
constexpr u8 SIZEOF_32 = sizeof(u32);
#endif
     
#if defined(__UINT64_TYPE__) && defined(__INT64_TYPE__)   
using u64 = __UINT64_TYPE__;    
using i64 = __INT64_TYPE__;
constexpr u64 U64_MAX = __UINT64_MAX__;
constexpr i64 I64_MAX = __INT64_MAX__;
constexpr i64 I64_MIN = -I64_MAX - 1;
constexpr u8 SIZEOF_64 = sizeof(u64);
#endif

#if defined(__SIZE_TYPE__) && defined(__PTRDIFF_TYPE__)
using dint = __SIZE_TYPE__;    
using sint = __PTRDIFF_TYPE__;
constexpr u64 DINT_MAX = __SIZE_MAX__;
constexpr i64 SINT_MAX = __PTRDIFF_MAX__;
constexpr i64 SINT_MIN = -SINT_MAX - 1;
constexpr u8 SIZEOF_LONGEST = sizeof(dint);
#endif

#ifdef __SIZEOF_INT128__
using u128 = unsigned __int128; 
using i128 = signed __int128;
constexpr u128 U128_MAX = (~(unsigned __int128)0);
constexpr i128 I128_MAX = ((unsigned __int128)~(unsigned __int128)0 >> 1);
constexpr i128 I128_MIN = -I128_MAX - 1;
#endif

#ifdef __FLT16_MANT_DIG__
using f16 = _Float16;
#endif

#ifdef __fp16
using fp16 = __fp16;
constexpr u8 SIZEOF_F16 = sizeof(f16);
#endif

#if __has_extension(c_bfloat16)
using bf16 = __bf16;
constexpr u8 SIZEOF_BF16 = sizeof(bf16);
#endif

using f32 = float;
constexpr u8 SIZEOF_F32 = sizeof(f32);
using f64 = double;
constexpr u8 SIZEOF_F64 = sizeof(f64);

#ifdef __FLOAT128__
using f128 = __float128;
constexpr u8 SIZEOF_F128 = sizeof(f128);
#endif

using c8 = unsigned char;
using sc8 = signed char;
using c16 = char16_t;
using c32 = char32_t;

template<typename T>
using ptr = T*;