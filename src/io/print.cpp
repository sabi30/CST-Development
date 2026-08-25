#include "macro.h"

#include <cstdarg>
#include <stdarg.h>

#include "io.h"
#include "type.h"

#ifdef CST_USE_LIBC
#include <stdio.h> // TESTING PURPOSE C STANDARD LIBRARY 2026
#endif


void cst::print(const char *text_format, ...) {
#ifdef CST_USE_LIBC
    va_list args;
    va_start(args, text_format);
    vprintf(text_format, args);
    va_end(args);
#endif
} 

int cst::scan(const char *text_format, ...) {
#ifdef CST_USE_LIBC
    va_list args;
    va_start(args, text_format);
    int result = vscanf(text_format, args);
    va_end(args);   
    return result;
#endif
}