#include "list.h"
#include "mem.h"
#include "mem_util.h"

void cst::list::copy(const list& other) {
    size = other.size; 
    capacity = other.capacity; 
    val_size = other.val_size;
    
    if (other.ptr && other.size > 0) {
        ptr = alloc(other.capacity * other.val_size);
        memcpy(ptr, other.ptr, other.size * other.val_size);
    }
}

void cst::list::move(list&& other) noexcept {
    ptr = other.ptr;
    size = other.size;
    capacity = other.capacity;
    val_size = other.val_size;
    
    other.ptr = nullptr;
    other.size = 0;
    other.capacity = 0;
}

dint cst::list::insert(const void *val) {
    if (size >= capacity) {
        capacity = capacity == 0 ? 4 : capacity * 2;
        void *new_list = realloc(ptr, capacity * val_size);
        if (!new_list) return -1;
        ptr = new_list;
    }

    char *dest = (char*)ptr + (size * val_size);
    memcpy(dest, val, val_size);
    return size++;
}

void cst::list::remove(const dint key) {
    if (key < 0 || key >= size) return;

    char *target = (char*)ptr + (key * val_size);
    char *next = target + val_size;
    dint bytes_to_move = (size - key - 1) * val_size;

    if (bytes_to_move > 0) {
        memmove(target, next, bytes_to_move);
    }
    size--;
}

void cst::list::insert_first(const void *val) {
    if (size >= capacity) {
        capacity = capacity == 0 ? 4 : capacity * 2;
        void *new_list = realloc(ptr, capacity * val_size);
        if (!new_list) return;
        ptr = new_list;
    }

    if (size > 0) {
        char *dest = (char*)ptr + val_size;
        memmove(dest, ptr, size * val_size);
    }

    memcpy(ptr, val, val_size);
    size++;
}

void cst::list::remove_first(const dint key) {
    remove(key);
}

void *cst::list::operator[](dint key) {
    if (key < 0 || key >= size) return nullptr;
    return (char*)ptr + (key * val_size);
}

const void *cst::list::operator[](dint key) const {
    if (key < 0 || key >= size) return nullptr;
    return (const char*)ptr + (key * val_size);
}

cst::list& cst::list::operator=(const list& other) {
    if (this != &other) {
        if (ptr) free(ptr);
        val_size = other.val_size;
        size = other.size;
        capacity = other.capacity;
        if (other.ptr && other.size > 0) {
            ptr = alloc(capacity * val_size);
            memcpy(ptr, other.ptr, size * val_size);
        } else {
            ptr = nullptr;
        }
    }
    return *this;
}

cst::list& cst::list::operator=(list&& other) noexcept {
    if (this != &other) {
        if (ptr) free(ptr);
        ptr = other.ptr;
        val_size = other.val_size;
        size = other.size;
        capacity = other.capacity;
        other.ptr = nullptr;
        other.size = 0;
        other.capacity = 0;
    }
    return *this;
}

cst::list& cst::list::operator<<(const list& other) {
    if (other.val_size != val_size) return *this;
    for (dint i = 0; i < other.size; i++) {
        insert(other[i]);
    }
    return *this;
}

void *cst::list::begin() {
    return ptr;
}

void *cst::list::end() {
    if (size <= 0) {
        return ptr;
    }
    return static_cast<u8*>(ptr) + (size * val_size);
}

void cst::list::init(dint s) {
    ptr = nullptr;
    size = 0;
    capacity = 0;
    val_size = s;
}

void cst::list::clear() {
    if (ptr) free(ptr);
    size = 0;
    capacity = 0;
}