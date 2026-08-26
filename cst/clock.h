#pragma once

#include "type.h"

namespace cst {
    u64 get_time_ns(void);
    u64 get_time_us(void);
    f64 get_time_sec(void);

    u64 get_cpu_cycles(void);

    void sleep_ns(u64 ns);
    void sleep_ms(u32 ms);
}