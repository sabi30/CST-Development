#include "str.h"

cst::string::string(const char *copy_cstr) {
    if (c_str != copy_cstr) {
        clear();
        if (copy_cstr) {
            size = strlen(copy_cstr);
            c_str = (char*)alloc(size + 1);
            memcpy(c_str, copy_cstr, size + 1);
        }
    }
}

cst::string::string(const string& other) {
    if (other.c_str && other.size > 0) {
        size = other.size;
        c_str = (char*)alloc(size + 1);
        memcpy(c_str, other.c_str, size + 1);
    }
}

cst::string::string(string&& other) noexcept : c_str(other.c_str), size(other.size) {
    other.c_str = nullptr;
    other.size = 0;
}

cst::string::~string(void) {
    clear();
}

cst::string& cst::string::operator=(const string& copy_string) {
    if (this != &copy_string) {
        clear();
        if (copy_string.c_str && copy_string.size > 0) {
            size = copy_string.size;
            c_str = (char*)alloc(size + 1);
            memcpy(c_str, copy_string.c_str, size + 1);
        }
    }
    return *this;
}

cst::string& cst::string::operator=(string&& other) noexcept {
    if (this != &other) {
        clear();
        c_str = other.c_str;
        size = other.size;
        other.c_str = nullptr;
        other.size = 0;
    }
    return *this;
}

cst::string& cst::string::operator=(const char *copy_cstr) {
    if (c_str != copy_cstr) {
        clear();
        if (copy_cstr) {
            size = strlen(copy_cstr);
            c_str = (char*)alloc(size + 1);
            memcpy(c_str, copy_cstr, size + 1);
        }
    }
    return *this;
}

bool cst::string::empty(void) const {
    return size == 0;
}

dint cst::string::length(void) const {
    return size;
}

char& cst::string::operator[](dint index) {
    return c_str[index];
}

const char& cst::string::operator[](dint index) const {
    return c_str[index];
}

const char* cst::string::data(void) const {
    return c_str ? c_str : "";
}

void cst::string::clear(void) {
    if (c_str) {
        free(c_str);
        c_str = nullptr;
    }
    size = 0;
}

void cst::string::add_cstr(const char *copy) {
    if (!copy) return;
    dint copy_size = strlen(copy);
    if (copy_size == 0) return;
    dint new_size = size + copy_size;

    char *new_cstr = (char*)realloc(c_str, new_size + 1);
    if (new_cstr) {
        c_str = new_cstr;
        memcpy(c_str + size, copy, copy_size + 1);
        size = new_size;
    }
}

cst::string& cst::string::operator+=(const char *copy_cstr) {
    add_cstr(copy_cstr);
    return *this;
}

cst::string& cst::string::operator+=(const string& other) {
    add_cstr(other.c_str);
    return *this;
}

cst::string cst::string::operator+(const char *copy_cstr) const {
    string result = *this;
    result += copy_cstr;
    return result;
}

cst::string cst::string::operator+(const string& other) const {
    string result = *this;
    result += other;
    return result;
}

bool cst::string::operator==(const string& other) const {
    if (size != other.size) return false;
    if (size == 0) return true;
    return strcmp(c_str, other.c_str) == 0;
}

bool cst::string::operator==(const char* other) const {
    if (!other) return empty();
    if (!c_str) return strlen(other) == 0;
    return strcmp(c_str, other) == 0;
}

bool cst::string::operator!=(const string& other) const {
    return !(*this == other);
}