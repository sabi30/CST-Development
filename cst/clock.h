#pragma once

#include "type.h"

namespace cst {
    u64 get_time_ns();
    u64 get_time_us();
    f64 get_time_sec();

    u64 get_cpu_cycles();

    void sleep_ns(u64 ns);
    void sleep_ms(u32 ms);
}