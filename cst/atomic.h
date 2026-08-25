#pragma once
#include "type.h"

namespace cst::atomic {
template <typename T>
struct alignas(sizeof(T)) atomic_raw {
    using type = _Atomic(T);
    volatile type value;
    
    inline void store(T desired, int order = 5) volatile {
        __c11_atomic_store(&value, desired, order);
    }
    
    inline T load(int order = 5) const volatile {
        return __c11_atomic_load(&value, order);
    }
    
    inline T fetch_add(T operand, int order = 5) volatile {
        return __c11_atomic_fetch_add(&value, operand, order);
    }
    
    inline bool compare_exchange(T& expected, T desired, int success_order = 5, int fail_order = 5) volatile {
        return __c11_atomic_compare_exchange_strong(&value, &expected, desired, success_order, fail_order);
    }
};

using atomic_u32 = atomic_raw<u32>;
using atomic_u64 = atomic_raw<u64>;
using atomic_ptr = atomic_raw<void*>;
}