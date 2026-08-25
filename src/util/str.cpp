#include "str_util.h"
#include "simd.h"
#include "mem_util.h"

dint cst::strlen(const char *string) {
    dint string_size = 0;
    dint current_index = 0;
    while (string[current_index]) {
        string_size++;
        current_index++;
    }

    return string_size;
};

bool cst::strcmp(const char *str1, const char *str2) {
    if (!str1 || !str2) {
        return str1 == str2;
    }

    dint i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return false;
        }
        i++; 
    }
    return str1[i] == '\0' && str2[i] == '\0';
}

sint cst::strfind(const char *str, const char *find_str) {
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

    if (len1 < 16 + len2 - 1) {
        for (dint i = 0; i <= max_i; ++i) {
            if (cst::memcmp(str + i, find_str, static_cast<dint>(len2)) == 0) {
                return i;
            }
        }
        return -1;
    }

    u8 n0 = static_cast<u8>(find_str[0]);
    u8 n1 = static_cast<u8>(find_str[1]);
    
    cst::simd::u8x16 vec_n0 = (cst::simd::u8x16)n0;
    cst::simd::u8x16 vec_n1 = (cst::simd::u8x16)n1;

    bool use_3byte = (len2 >= 3);
    u8 n2 = use_3byte ? static_cast<u8>(find_str[2]) : 0;
    cst::simd::u8x16 vec_n2 = (cst::simd::u8x16)n2;

    dint i = 0;

    for (; i + 15 <= max_i; i += 16) {
        cst::simd::u8x16 t0, t1, t2;

        __builtin_memcpy(&t0, str + i, 16);
        __builtin_memcpy(&t1, str + i + 1, 16);

       cst::simd::u8x16 match = (cst::simd::u8x16)(t0 == vec_n0) & (cst::simd::u8x16)(t1 == vec_n1);

        if (use_3byte) {
            __builtin_memcpy(&t2, str + i + 2, 16);
            match &= (cst::simd::u8x16)(t2 == vec_n2);
        }

        cst::simd::i64x2 mask64 = (cst::simd::i64x2)match;
        if (mask64[0] | mask64[1]) {
            for (int lane = 0; lane < 16; ++lane) {
                if (match[lane]) {
                    dint pos = i + lane;
                    if (pos <= max_i && cst::memcmp(str + pos, find_str, static_cast<dint>(len2)) == 0) {
                        return pos;
                    }
                }
            }
        }
    }

    for (; i <= max_i; ++i) {
        if (cst::memcmp(str + i, find_str, static_cast<dint>(len2)) == 0) {
            return i;
        }
    }

    return -1;
}
