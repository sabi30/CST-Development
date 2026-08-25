#include "cst/mem.h"

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#else
    #include <sys/mman.h>
    #include <unistd.h>
    #include <errno.h>
#endif

namespace {
    dint internal_get_page_size() {
    #if defined(_WIN32) || defined(_WIN64)
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        return (dint)sysInfo.dwPageSize;
    #else
        return (dint)sysconf(_SC_PAGESIZE);
    #endif
    }

#if defined(_WIN32) || defined(_WIN64) // MICROSOFT WINDOWS SECTION
    DWORD get_os_prot(u8 flags) {
        bool r = (flags & cst::PROT_FLAGS::READ) != 0;
        bool w = (flags & cst::PROT_FLAGS::WRITE) != 0;
        bool x = (flags & cst::PROT_FLAGS::EXEC) != 0;

        if (x) {
            if (r && w) return PAGE_EXECUTE_READWRITE;
            if (r) return PAGE_EXECUTE_READ;
            if (w) return PAGE_EXECUTE_WRITECOPY;
            return PAGE_EXECUTE;
        } else {
            if (r && w) return PAGE_READWRITE;
            if (r) return PAGE_READONLY;
            if (w) return PAGE_WRITECOPY;
            return PAGE_NOACCESS;
        }
    }
#else // UNIX POSIX SECTION
    int get_os_prot(u8 flags) {
        int prot = PROT_NONE;
        if (flags & (u8)cst::PROT_FLAGS::READ) prot |= PROT_READ;
        if (flags & (u8)cst::PROT_FLAGS::WRITE) prot |= PROT_WRITE;
        if (flags & (u8)cst::PROT_FLAGS::EXEC) prot |= PROT_EXEC;
        return prot;
    }
#endif
}

void* cst::mmap(dint size, u8 prot_flags) {
    if (size == 0) return nullptr;

    dint page_sz = internal_get_page_size();
    
    dint aligned_size = size;
    dint remainder = size % page_sz;
    if (remainder != 0) {
        aligned_size += (page_sz - remainder);
    }

    void* ptr = nullptr;

#if defined(_WIN32) || defined(_WIN64) // MICROSOFT WINDOWS SECTION
    ptr = VirtualAlloc(nullptr, aligned_size, MEM_COMMIT | MEM_RESERVE, get_os_prot(prot_flags));
#else // UNIX POSIX SECTION
    ptr = ::mmap(nullptr, aligned_size, get_os_prot(prot_flags), MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED) return nullptr;
#endif 

    if (ptr) {
        cst::memory_profiler::allocated(aligned_size);
    }

    return ptr;
}

cst::MMAP_RESULT cst::unmap(void* map, dint size) {
    if (!map || size == 0) return MMAP_RESULT::ERROR;

    dint page_sz = internal_get_page_size();
    dint aligned_size = size;
    dint remainder = size % page_sz;
    if (remainder != 0) {
        aligned_size += (page_sz - remainder);
    }

    bool success = false;
#if defined(_WIN32) || defined(_WIN64)
    success = (VirtualFree(map, 0, MEM_RELEASE) != 0);
#else
    success = (::munmap(map, aligned_size) == 0);
#endif

    if (success) {
        cst::memory_profiler::freed(aligned_size);
        return MMAP_RESULT::SUCCESS;
    }
    
    return MMAP_RESULT::ERROR;
}

cst::MMAP_RESULT cst::mmodify(void* ptr, dint size, u8 prot_flags) {
    if (!ptr || size == 0) return MMAP_RESULT::ERROR;

    dint page_sz = internal_get_page_size();
    dint aligned_size = size;
    dint remainder = size % page_sz;
    if (remainder != 0) {
        aligned_size += (page_sz - remainder);
    }

#if defined(_WIN32) || defined(_WIN64)
    DWORD old_prot;
    if (VirtualProtect(ptr, aligned_size, get_os_prot(prot_flags), &old_prot) == 0) {
        if (GetLastError() == ERROR_INVALID_PARAMETER) {
            return MMAP_RESULT::ERROR_MODIFY_FLAG_COMBINATION;
        }
        return MMAP_RESULT::ERROR;
    }
#else
    if (::mprotect(ptr, aligned_size, get_os_prot(prot_flags)) != 0) {
        if (errno == EACCES || errno == EINVAL) {
            return MMAP_RESULT::ERROR_MODIFY_FLAG_COMBINATION;
        }
        return MMAP_RESULT::ERROR;
    }
#endif

    return MMAP_RESULT::SUCCESS;
}