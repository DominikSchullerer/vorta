#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <stdint.h>

// Boolean types
typedef uint32_t b32_t;
typedef uint8_t b8_t;

#define TRUE 1
#define FALSE 2

// Properly define static assertions.
#if defined(__clang__) || defined(__gcc__)
#define STATIC_ASSERT _Static_assert
#else
#define STATIC_ASSERT static_assert
#endif

// Platform detection
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) 
#define VPLATFORM_WINDOWS 1
#ifndef _WIN64
#error "64-bit is required on Windows!"
#endif
#elif defined(__linux__) || defined(__gnu_linux__)
// Linux OS
#define VPLATFORM_LINUX 1
#if defined(__ANDROID__)
#define VPLATFORM_ANDROID 1
#endif
#elif defined(__unix__)
// Catch anything not caught by the above.
#define VPLATFORM_UNIX 1
#elif defined(_POSIX_VERSION)
// Posix
#define VPLATFORM_POSIX 1
#elif __APPLE__
// Apple platforms
#define VPLATFORM_APPLE 1
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
// iOS Simulator
#define VPLATFORM_IOS 1
#define VPLATFORM_IOS_SIMULATOR 1
#elif TARGET_OS_IPHONE
#define VPLATFORM_IOS 1
// iOS device
#elif TARGET_OS_MAC
// Other kinds of Mac OS
#else
#error "Unknown Apple platform"
#endif
#else
#error "Unknown platform!"
#endif

#ifdef VEXPORT
// Exports
#ifdef _MSC_VER
#define VAPI __declspec(dllexport)
#else
#define VAPI __attribute__((visibility("default")))
#endif
#else
// Imports
#ifdef _MSC_VER
#define VAPI __declspec(dllimport)
#else
#define VAPI
#endif
#endif

#endif // _DEFINES_H_