#pragma once

namespace cst {
    template<typename t, typename... args>
    using function = t (*)(args...);
}