#include "sort.h"

template <typename T>
void swap(T &a, T &b) {
    T temp = a;
    a = b;
    b = temp;
}

int cstr_compare(const char *s1, const char *s2) {
    if (s1 == s2) return 0;
    if (!s1) return -1;
    if (!s2) return 1;

    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return static_cast<unsigned char>(*s1) - static_cast<unsigned char>(*s2);
}

template <typename T, typename Compare>
void quick_sort_t(T *begin, T *end, Compare comp) {
    if (end - begin <= 1) return;

    T *i = begin;
    T *j = end - 1;
    T pivot = *(begin + (end - begin) / 2);

    while (i <= j) {
        while (comp(*i, pivot)) i++;
        while (comp(pivot, *j)) j--;

        if (i <= j) {
            swap(*i, *j);
            i++;
            if (j > begin) j--;
        }
    }

    if (begin < j + 1) quick_sort_t(begin, j + 1, comp);
    if (i < end) quick_sort_t(i, end, comp);
}


void cst::number_sort(f64 *begin, f64 *end, SORT_STYLE style) {
    if (!begin || !end || begin >= end) return;

    if (style == SORT_STYLE::ASCENDING) {
        quick_sort_t(begin, end, [](f64 a, f64 b) { return a < b; });
    } else {
        quick_sort_t(begin, end, [](f64 a, f64 b) { return a > b; });
    }
}

void cst::string_sort(void *begin, void *end, SORT_STYLE style) {
    if (!begin || !end || begin >= end) return;

    auto s_begin = static_cast<const char**>(begin);
    auto s_end = static_cast<const char**>(end);

    if (style == SORT_STYLE::ASCENDING) {
        quick_sort_t(s_begin, s_end, [](const char *a, const char *b) {
            return cstr_compare(a, b) < 0;
        });
    } else {
        quick_sort_t(s_begin, s_end, [](const char *a, const char *b) {
            return cstr_compare(a, b) > 0;
        });
    }
}