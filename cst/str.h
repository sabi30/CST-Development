#pragma once

#include "type.h"
#include "mem.h"
#include "mem_util.h"

#include "str_util.h"

namespace cst {

    struct string {
        char *c_str = nullptr;
        dint size = 0;

        string(void) : c_str(nullptr), size(0) {};
        string(const char* s);
        string(const string& other);
        string(string&& other) noexcept;
        ~string(void);
    
        string& operator=(const string& copy_string);
        string& operator=(string&& other) noexcept;
        string& operator=(const char *copy_cstr);
    
        bool empty(void) const;
        dint length(void) const;
        char& operator[](dint index);
        const char& operator[](dint index) const;
        const char* data() const;
    
        void clear(void);
        void add_cstr(const char *copy);
    
        string& operator+=(const char *copy_cstr);
        string& operator+=(const string& other);
        string operator+(const char *copy_cstr) const;
        string operator+(const string& other) const;
    
        bool operator==(const string& other) const;
        bool operator==(const char* other) const;
        bool operator!=(const string& other) const;
    };

}