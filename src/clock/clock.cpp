#include "cst/clock.h"

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#else
    #include <time.h>
    #include <unistd.h>
#endif

#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
    #if defined(_MSC_VER)
        #include <intrin.h>
    #else
        #include <x86intrin.h>
    #endif
#endif

namespace {
#if defined(_WIN32) || defined(_WIN64)
    u64 internal_get_qpc_freq() {
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        return (u64)freq.QuadPart;
    }
#endif
}

u64 cst::get_time_ns() {
#if defined(_WIN32) || defined(_WIN64)
    static u64 freq = internal_get_qpc_freq();
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return ((u64)counter.QuadPart * 1000000000ULL) / freq;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ((u64)ts.tv_sec * 1000000000ULL) + (u64)ts.tv_nsec;
#endif
}

u64 cst::get_time_us() {
    return cst::get_time_ns() / 1000ULL;
}

f64 cst::get_time_sec() {
    return (f64)cst::get_time_ns() / 1000000000.0;
}

u64 cst::get_cpu_cycles() {
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
    return __rdtsc();
#elif defined(__aarch64__)
    u64 val;
    asm volatile("mrs %0, cntvct_el0" : "=r"(val));
    return val;
#else
    return cst::get_time_ns();
#endif
}

void cst::sleep_ns(u64 ns) {
#if defined(_WIN32) || defined(_WIN64)
    u64 start = cst::get_time_ns();
    while ((cst::get_time_ns() - start) < ns) {
        #if defined(__x86_64__) || defined(_M_X64)
            _mm_pause(); 
        #endif
    }
#else
    struct timespec req;
    req.tv_sec = (time_t)(ns / 1000000000ULL);
    req.tv_nsec = (long)(ns % 1000000000ULL);
    nanosleep(&req, nullptr);
#endif
}

void cst::sleep_ms(u32 ms) {
#if defined(_WIN32) || defined(_WIN64)
    Sleep((DWORD)ms);
#else
    cst::sleep_ns((u64)ms * 1000000ULL);
#endif
}