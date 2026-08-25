#pragma once

namespace cst {
    void exit();
    void abort();

    const char *get_env(const char*);
    unsigned int get_pid();
}