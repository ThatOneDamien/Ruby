#ifdef _WIN32
        #ifndef RB_GMAKE
            #define RB_NOVTABLE __declspec(novtable)
            #define RB_NO_DISCARD [[nodiscard]]
        #else
            #define RB_NOVTABLE
            #define RB_NO_DISCARD
        #endif
    #ifdef _WIN64
        // Windows x64 builds.
        #define RB_PLAT_WIND 1
        #define RB_DEBUG_BREAK() __debugbreak()

    #else
        // Windows x86 builds.
        #error "x86/32-bit builds are not supported."
    #endif

#elif __linux__
    #define RB_PLAT_LINUX 1
    #include <signal.h>
    #define RB_DEBUG_BREAK() raise(SIGTRAP)

    #define RB_NOVTABLE
    #define RB_NO_DISCARD
#else
    // Other builds. TODO handle more systems specifically.
    #error "Platform not currently supported."
    #define RB_NOVTABLE  
#endif
