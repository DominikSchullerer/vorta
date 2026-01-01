#ifndef _DEFINES_H_
    #define _DEFINES_H_

    #include <stdint.h>

    typedef float float32_t;
    typedef double float64_t;

    // Boolean types
    typedef uint32_t bool32_t;
    typedef uint8_t bool8_t;

    #define TRUE 1
    #define FALSE 0

    // Properly define static assertions.
    #if defined(__clang__) || defined(__gcc__)
        #define STATIC_ASSERT _Static_assert
    #else
        #define STATIC_ASSERT static_assert
    #endif

    // Platform detection
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
        #define V_PLATFORM_WINDOWS 1
        #ifndef _WIN64
            #error "64-bit is required on Windows!"
        #endif
    #elif defined(__linux__) || defined(__gnu_linux__)
        // Linux OS
        #define V_PLATFORM_LINUX 1
        #if defined(__ANDROID__)
            #define V_PLATFORM_ANDROID 1
        #endif
    #elif defined(__unix__)
        // Catch anything not caught by the above.
        #define V_PLATFORM_UNIX 1
    #elif defined(_POSIX_VERSION)
        // Posix
        #define V_PLATFORM_POSIX 1
    #elif __APPLE__
        // Apple platforms
        #define V_PLATFORM_APPLE 1
        #include <TargetConditionals.h>
        #if TARGET_IPHONE_SIMULATOR
            // iOS Simulator
            #define V_PLATFORM_IOS 1
            #define V_PLATFORM_IOS_SIMULATOR 1
        #elif TARGET_OS_IPHONE
            // iOS device
            #define V_PLATFORM_IOS 1
        #elif TARGET_OS_MAC
            // Other kinds of Mac OS
        #else
            #error "Unknown Apple platform"
        #endif
    #else
        #error "Unknown platform!"
    #endif

    #ifdef V_EXPORT
        // Exports
        #ifdef _MSC_VER
            #define V_API __declspec(dllexport)
        #else
            #define V_API __attribute__((visibility("default")))
        #endif
    #else
        // Imports
        #ifdef _MSC_VER
            #define V_API __declspec(dllimport)
        #else
            #define V_API
        #endif
    #endif

#endif  // _DEFINES_H_