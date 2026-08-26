#pragma once

#include "list.h"

namespace cst {

    struct map {
        list data;

        void init(dint key_size, dint val_size);
        void clear(void);

        void *get(void *key);
        dint insert(void *key, void *val);
        void remove(void *key);

        void *operator[](void *key);
    };
}