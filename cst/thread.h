#pragma once

#include "type.h"

namespace cst {
    using THREAD_FUNC = void *(*)(void *args);
    using thread_t = sint;

    thread_t spawn_thread(THREAD_FUNC, void *args);
    void *join_thread(thread_t);
    bool detach_thread(thread_t);

    unsigned int get_tid();
}