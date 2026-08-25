#include "cst/proc.h"

#if defined(_WIN32) || defined(_WIN64) // MICROSOFT WINDOWS SECTION
    #include <windows.h>
#else // UNIX POSIX SECTION
    #include <unistd.h>
    extern char** environ;
#endif

void cst::exit() {
#if defined(_WIN32) || defined(_WIN64)
    ExitProcess(0); 
#else
    _exit(0);       
#endif
}

void cst::abort() {
#if defined(_WIN32) || defined(_WIN64)
    RaiseFailFastException(nullptr, nullptr, 0);
#else
    __builtin_trap(); 
#endif
}

const char* cst::get_env(const char* name) {
    if (!name) return nullptr;

#if defined(_WIN32) || defined(_WIN64) // MICROSOFT WINDOWS SECTION
    LPCH envBlock = GetEnvironmentStringsA();
    if (!envBlock) return nullptr;

    const char* current = envBlock;
    const char* result = nullptr;

    while (*current != '\0') {
        const char* eq = current;
        while (*eq != '=' && *eq != '\0') {
            eq++;
        }

        if (*eq == '=') {
            dint key_len = eq - current;

            bool match = true;
            for (dint i = 0; i < key_len; ++i) {
                if (name[i] == '\0' || current[i] != name[i]) {
                    match = false;
                    break;
                }
            }
            if (match && name[key_len] == '\0') {
                result = eq + 1; 
                break;
            }
        }

        while (*current != '\0') {
            current++;
        }
        current++;
    }

    FreeEnvironmentStringsA(envBlock);
    return result;
#else // UNIX POSIX SECTION
    if (!environ) return nullptr;
    
    for (int i = 0; environ[i] != nullptr; ++i) {
        const char* env_str = environ[i];
        const char* name_ptr = name;
        
        while (*env_str != '=' && *env_str != '\0' && *name_ptr != '\0' && *env_str == *name_ptr) {
            env_str++;
            name_ptr++;
        }

        if (*env_str == '=' && *name_ptr == '\0') {
            return env_str + 1;
        }
    }

    return nullptr;
#endif
}

unsigned int cst::get_pid() {
    return getpid();
}