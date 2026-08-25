#pragma once

#include "type.h"

namespace cst {
    using file_handle = sint;
    const file_handle INVALID_FILE_HANDLE = -1;

    using dir_handle = sint;
    constexpr dir_handle INVALID_DIR_HANDLE = -1;

    struct dir_entry {
        char name[256];
        bool is_directory;
        dint size;
    };


    enum class FILE_MODE : u8 {
        READ,
        WRITE,
        READ_WRITE,
        APPEND,
    };

    enum class SEEK_MODE : u8 {
        SET, 
        CUR, 
        END  
    };

    file_handle open_file(const char *path, FILE_MODE mode = FILE_MODE::READ);
    void close_file(file_handle);

    dint read_file(file_handle handle, void* buffer, dint bytes_to_read);
    dint write_file(file_handle handle, const void* buffer, dint bytes_to_write);

    dint seek_file(file_handle handle, dint offset, SEEK_MODE origin);
    dint get_file_size(file_handle handle);

    bool file_exists(const char* path);
    
    dir_handle open_dir(const char* path);
    bool read_dir(dir_handle handle, dir_entry* out_entry);
    void close_dir(dir_handle handle);
}