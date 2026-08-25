#pragma once

#include "list.h"

namespace cst {

    struct map {
        list data;

        map(dint key_size, dint value_size);
        ~map();

        void *get(void *key);
        dint insert(void *key, void *val);
        void remove(void *key);

        void *operator[](void *key);
    };
}