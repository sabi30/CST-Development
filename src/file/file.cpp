#include "cst/file.h"

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>

    struct Win32DirState {
        HANDLE hFind;
        WIN32_FIND_DATAA findData;
        bool has_first;
    };
#else
    #include <fcntl.h>
    #include <unistd.h>
    #include <dirent.h>
    #include <sys/stat.h>
#endif

cst::file_handle cst::open_file(const char* path, FILE_MODE mode) {
    if (!path) return INVALID_FILE_HANDLE;

#if defined(_WIN32) || defined(_WIN64)
    DWORD access = 0;
    DWORD creation = 0;

    switch (mode) {
        case FILE_MODE::READ:
            access = GENERIC_READ;
            creation = OPEN_EXISTING;
            break;
        case FILE_MODE::WRITE:
            access = GENERIC_WRITE;
            creation = CREATE_ALWAYS;
            break;
        case FILE_MODE::READ_WRITE:
            access = GENERIC_READ | GENERIC_WRITE;
            creation = OPEN_ALWAYS;
            break;
        case FILE_MODE::APPEND:
            access = FILE_APPEND_DATA;
            creation = OPEN_ALWAYS;
            break;
    }

    HANDLE handle = CreateFileA(
        path, access, FILE_SHARE_READ, nullptr,
        creation, FILE_ATTRIBUTE_NORMAL, nullptr
    );

    if (handle == INVALID_HANDLE_VALUE) return INVALID_FILE_HANDLE;
    return (cst::file_handle)handle;
#else
    int flags = 0;

    switch (mode) {
        case FILE_MODE::READ:       flags = O_RDONLY; break;
        case FILE_MODE::WRITE:      flags = O_WRONLY | O_CREAT | O_TRUNC; break;
        case FILE_MODE::READ_WRITE: flags = O_RDWR | O_CREAT; break;
        case FILE_MODE::APPEND:     flags = O_WRONLY | O_CREAT | O_APPEND; break;
    }

    int fd = ::open(path, flags, 0644);
    if (fd < 0) return INVALID_FILE_HANDLE;
    return (cst::file_handle)fd;
#endif
}

void cst::close_file(file_handle handle) {
    if (handle == INVALID_FILE_HANDLE) return;

#if defined(_WIN32) || defined(_WIN64)
    CloseHandle((HANDLE)handle);
#else
    ::close((int)handle);
#endif
}

dint cst::read_file(file_handle handle, void* buffer, dint bytes_to_read) {
    if (handle == INVALID_FILE_HANDLE || !buffer || bytes_to_read <= 0) return -1;

#if defined(_WIN32) || defined(_WIN64)
    DWORD bytes_read = 0;
    if (!ReadFile((HANDLE)handle, buffer, (DWORD)bytes_to_read, &bytes_read, nullptr)) {
        return -1;
    }
    return (dint)bytes_read;
#else
    sint res = ::read((int)handle, buffer, (dint)bytes_to_read);
    return (dint)res;
#endif
}

dint cst::write_file(file_handle handle, const void* buffer, dint bytes_to_write) {
    if (handle == INVALID_FILE_HANDLE || !buffer || bytes_to_write <= 0) return -1;

#if defined(_WIN32) || defined(_WIN64)
    DWORD bytes_written = 0;
    if (!WriteFile((HANDLE)handle, buffer, (DWORD)bytes_to_write, &bytes_written, nullptr)) {
        return -1;
    }
    return (dint)bytes_written;
#else
    sint res = ::write((int)handle, buffer, (dint)bytes_to_write);
    return (dint)res;
#endif
}

dint cst::seek_file(file_handle handle, dint offset, SEEK_MODE origin) {
    if (handle == INVALID_FILE_HANDLE) return -1;

#if defined(_WIN32) || defined(_WIN64)
    DWORD method = FILE_BEGIN;
    if (origin == SEEK_MODE::CUR) method = FILE_CURRENT;
    if (origin == SEEK_MODE::END) method = FILE_END;

    LARGE_INTEGER li_offset;
    li_offset.QuadPart = offset;
    LARGE_INTEGER li_new_pos;

    if (!SetFilePointerEx((HANDLE)handle, li_offset, &li_new_pos, method)) {
        return -1;
    }
    return (dint)li_new_pos.QuadPart;
#else
    int whence = SEEK_SET;
    if (origin == SEEK_MODE::CUR) whence = SEEK_CUR;
    if (origin == SEEK_MODE::END) whence = SEEK_END;

    off_t res = ::lseek((int)handle, (off_t)offset, whence);
    return (dint)res;
#endif
}

dint cst::get_file_size(file_handle handle) {
    if (handle == INVALID_FILE_HANDLE) return -1;

#if defined(_WIN32) || defined(_WIN64)
    LARGE_INTEGER size;
    if (!GetFileSizeEx((HANDLE)handle, &size)) return -1;
    return (dint)size.QuadPart;
#else
    struct stat st;
    if (fstat((int)handle, &st) != 0) return -1;
    return (dint)st.st_size;
#endif
}

bool cst::file_exists(const char* path) {
    if (!path) return false;

#if defined(_WIN32) || defined(_WIN64)
    DWORD dwAttrib = GetFileAttributesA(path);
    return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
#else
    struct stat st;
    return (stat(path, &st) == 0 && !S_ISDIR(st.st_mode));
#endif
}

cst::dir_handle cst::open_dir(const char* path) {
    if (!path) return INVALID_DIR_HANDLE;

#if defined(_WIN32) || defined(_WIN64)
    char search_path[MAX_PATH];
    dint i = 0;
    while (path[i] != '\0' && i < MAX_PATH - 3) {
        search_path[i] = path[i];
        i++;
    }
    if (i > 0 && search_path[i - 1] != '/' && search_path[i - 1] != '\\') {
        search_path[i++] = '\\';
    }
    search_path[i++] = '*';
    search_path[i] = '\0';

    Win32DirState* state = (Win32DirState*)cst::mmap(sizeof(Win32DirState));
    if (!state) return INVALID_DIR_HANDLE;

    state->hFind = FindFirstFileA(search_path, &state->findData);
    
    if (state->hFind == INVALID_HANDLE_VALUE) {
        cst::unmap(state, sizeof(Win32DirState));
        return INVALID_DIR_HANDLE;
    }

    state->has_first = true;
    return (cst::dir_handle)state;
#else
    DIR* dir = ::opendir(path);
    if (!dir) return INVALID_DIR_HANDLE;
    return (cst::dir_handle)dir;
#endif
}

bool cst::read_dir(dir_handle handle, dir_entry* out_entry) {
    if (handle == INVALID_DIR_HANDLE || !out_entry) return false;

#if defined(_WIN32) || defined(_WIN64)
    Win32DirState* state = (Win32DirState*)handle;

    while (true) {
        if (!state->has_first) {
            if (!FindNextFileA(state->hFind, &state->findData)) {
                return false;
            }
        }
        state->has_first = false;

        const char* name = state->findData.cFileName;
        if (name[0] == '.' && (name[1] == '\0' || (name[1] == '.' && name[2] == '\0'))) {
            continue;
        }

        dint i = 0;
        while (name[i] != '\0' && i < 255) {
            out_entry->name[i] = name[i];
            i++;
        }
        out_entry->name[i] = '\0';

        out_entry->is_directory = (state->findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
        
        LARGE_INTEGER sz;
        sz.HighPart = state->findData.nFileSizeHigh;
        sz.LowPart = state->findData.nFileSizeLow;
        out_entry->size = (dint)sz.QuadPart;

        return true;
    }
#else
    DIR* dir = (DIR*)handle;
    struct dirent* entry = nullptr;

    while ((entry = ::readdir(dir)) != nullptr) {
        if (entry->d_name[0] == '.' && (entry->d_name[1] == '\0' || (entry->d_name[1] == '.' && entry->d_name[2] == '\0'))) {
            continue;
        }

        dint i = 0;
        while (entry->d_name[i] != '\0' && i < 255) {
            out_entry->name[i] = entry->d_name[i];
            i++;
        }
        out_entry->name[i] = '\0';

        out_entry->is_directory = (entry->d_type == DT_DIR);
        out_entry->size = 0; 

        return true;
    }

    return false;
#endif
}

void cst::close_dir(dir_handle handle) {
    if (handle == INVALID_DIR_HANDLE) return;

#if defined(_WIN32) || defined(_WIN64)
    Win32DirState* state = (Win32DirState*)handle;
    FindClose(state->hFind);
    cst::unmap(state, sizeof(Win32DirState));
#else
    ::closedir((DIR*)handle);
#endif
}