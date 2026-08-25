#pragma once

#include "type.h"

namespace cst {
    dint strlen(const char *string);
    bool strcmp(const char *str1, const char *str2);
    sint strfind(const char *str, const char *find_str);
}