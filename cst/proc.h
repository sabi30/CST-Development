#pragma once

namespace cst {
    void exit(void);
    void abort(void);

    const char *get_env(const char*);
    unsigned int get_pid(void);
}