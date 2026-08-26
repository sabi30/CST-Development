#include "str_util.h"
#include "simd.h"
#include "mem_util.h"

dint cst::strlen(const char *string) {
    if (!string) return 0;
    const char *p = string;

    while (true) {
        cst::simd::u8x16 v;
        __builtin_memcpy(&v, p, 16);

        cst::simd::u8x16 zero_vec = {0};
        cst::simd::u8x16 cmp = (v == zero_vec);
        cst::simd::i64x2 mask64 = reinterpret_cast<cst::simd::i64x2>(cmp);

        if (mask64[0] | mask64[1]) {
            for (int lane = 0; lane < 16; ++lane) {
                if (p[lane] == '\0') {
                    return static_cast<dint>(p + lane - string);
                }
            }
        }
        p += 16;
    }
}

bool cst::strcmp(const char *str1, const char *str2) {
    if (!str1 || !str2) return str1 == str2;

    dint i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) return false;
        i++; 
    }
    return str1[i] == '\0' && str2[i] == '\0';
}

sint cst::strfind(const char *str, const char *find_str) {
    if (!str || !find_str) return -1;

    dint len1 = cst::strlen(str);
    dint len2 = cst::strlen(find_str);

    if (len2 == 0) return 0;
    if (len1 < len2) return -1;

    dint max_i = len1 - len2;

    if (len2 == 1) {
        char c = find_str[0];
        for (dint i = 0; i <= max_i; ++i) {
            if (str[i] == c) return i;
        }
        return -1;
    }

    if (len1 < 16 + len2) {
        for (dint i = 0; i <= max_i; ++i) {
            if (cst::memcmp(str + i, find_str, len2) == 0) return i;
        }
        return -1;
    }

    u8 n0 = static_cast<u8>(find_str[0]);
    u8 n1 = static_cast<u8>(find_str[1]);
    
    cst::simd::u8x16 vec_n0, vec_n1;
    for (int j = 0; j < 16; ++j) {
        vec_n0[j] = n0;
        vec_n1[j] = n1;
    }

    dint i = 0;
    for (; i + 15 + 1 <= len1; i += 16) {
        cst::simd::u8x16 t0, t1;

        __builtin_memcpy(&t0, str + i, 16);
        __builtin_memcpy(&t1, str + i + 1, 16);

        cst::simd::u8x16 match = (t0 == vec_n0) & (t1 == vec_n1);
        cst::simd::i64x2 mask64 = reinterpret_cast<cst::simd::i64x2>(match);

        if (mask64[0] | mask64[1]) {
            for (int lane = 0; lane < 16; ++lane) {
                if (match[lane]) {
                    dint pos = i + lane;
                    if (pos <= max_i && cst::memcmp(str + pos, find_str, len2) == 0) {
                        return pos;
                    }
                }
            }
        }
    }

    for (; i <= max_i; ++i) {
        if (cst::memcmp(str + i, find_str, len2) == 0) {
            return i;
        }
    }

    return -1;
}