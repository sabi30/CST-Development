#include "macro.h"

#include "cst/thread.h"

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#else
#ifdef CST_USE_LIBC
    #include <unistd.h>
    #include <pthread.h>
#endif
#endif

cst::thread_t cst::spawn_thread(THREAD_FUNC func, void* args) {
    if (!func) return 0;

#if defined(_WIN32) || defined(_WIN64) // MICROSOFT WINDOWS SECTION
    HANDLE hThread = CreateThread(
        nullptr,
        0,
        (LPTHREAD_START_ROUTINE)func,
        args,
        0,
        nullptr
    );
    return (cst::thread_t)hThread;
#else // UNIX POSIX SECTION
#ifdef CST_USE_LIBC
    pthread_t thread;
    if (pthread_create(&thread, nullptr, func, args) != 0) {
        return 0;
    }
    return (cst::thread_t)thread;
#endif
#endif
}

void* cst::join_thread(thread_t thread) {
    if (!thread) return nullptr;

#if defined(_WIN32) || defined(_WIN64) // MICROSOFT WINDOWS SECTION
    HANDLE hThread = (HANDLE)thread;
    if (WaitForSingleObject(hThread, INFINITE) != WAIT_OBJECT_0) {
        CloseHandle(hThread);
        return nullptr;
    }

    DWORD exit_code = 0;
    GetExitCodeThread(hThread, &exit_code);
    CloseHandle(hThread); 

    return (void*)(uintptr_t)exit_code;
#else // UNIX POSIX SECTION
#ifdef CST_USE_LIBC
    pthread_t pthread = (pthread_t)thread;
    void* return_val = nullptr;

    if (pthread_join(pthread, &return_val) != 0) {
        return nullptr;
    }

    return return_val;
#endif
#endif
}

bool cst::detach_thread(thread_t thread) {
    if (!thread) return false;

#if defined(_WIN32) || defined(_WIN64) // MICROSOFT WINDOWS SECTION
    HANDLE hThread = (HANDLE)thread;
    return (CloseHandle(hThread) != 0);
#else // UNIX POSIX SECTION
#ifdef CST_USE_LIBC
    pthread_t pthread = (pthread_t)thread;
    return (pthread_detach(pthread) == 0);
#endif
#endif
}

unsigned int cst::get_tid() {
#ifdef CST_USE_LIBC
    return gettid();
#endif
    return 0;
}