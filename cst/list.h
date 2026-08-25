#pragma once
#include "type.h"

namespace cst {

    // List (aka Vector)
    struct list {
        void *ptr = nullptr;
        dint size = 0;
        dint capacity = 0;
        dint val_size = 0;

        void init(dint);
        void clear();

        void copy(const list& other);
        void move(list&& other) noexcept;

        dint insert(const void *val);
        void remove(const dint key);

        void insert_first(const void *val);
        void remove_first(const dint key);

        void *begin();
        void *end();
    
        void *operator[](dint key);
        const void *operator[](dint) const;

        list& operator=(const list&);
        list& operator=(list&& other) noexcept;
        list& operator<<(const list&);
    };

    // Template Wrapper of List, handles casting
    template<typename T>
    struct list_t {
        list data;

        void init() {
            data.init(sizeof(T));
        }

        void clear() {
            data.clear();
        }
            
        dint insert(const T val) {
            return data.insert(&val);
        }

        void remove(const dint key) {
            data.remove(key);
        }

        void insert_first(const T val) {
            data.insert_first(&val);
        }

        void remove_first(const dint key) {
            data.remove_first(key);
        }

        T *operator[](dint key) {
            return static_cast<T*>(data[key]);
        }

        const T *operator[](dint key) const {
            return static_cast<T*>(data[key]);
        }

        list& operator=(const list& other) {
            data = other;
            return *this;
        };

        list& operator=(list&& other) noexcept {
            data = other;
            return *this;
        };

        list& operator<<(const list& other) {
            data << other;
            return *this;
        };

        T *begin() {
            return static_cast<T*>(data.ptr);
        };

        T *end() {
            return static_cast<T*>(data.end());
        }   
        
        dint size() {
            return data.size;
        };
    };


}
