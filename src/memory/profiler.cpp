#include "mem.h"

namespace {
    dint submit_count = 0;
    dint MEMORY_ALLTIME_USAGE = 0;
}

namespace cst::memory_profiler {
    dint PEAK_MEMORY_USAGE = 0;
    dint AVG_MEMORY_USAGE = 0;
    dint MEMORY_USAGE = 0;
}

void cst::memory_profiler::allocated(dint size) {  
    MEMORY_USAGE += size;
    MEMORY_ALLTIME_USAGE += size;
    if (PEAK_MEMORY_USAGE < MEMORY_USAGE) {
        PEAK_MEMORY_USAGE = MEMORY_USAGE;
    }

    submit_count++;
    AVG_MEMORY_USAGE = MEMORY_ALLTIME_USAGE / submit_count;
}

void cst::memory_profiler::freed(dint size) {
    MEMORY_USAGE -= size;

    submit_count++;
    AVG_MEMORY_USAGE = MEMORY_ALLTIME_USAGE / submit_count;
}
