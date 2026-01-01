#ifndef _ASSERTS_H_

    #include "defines.h"

    #define _ASSERTS_H_

    #define V_ASSERTIONS_ENABLED 1

    #if V_ASSERTIONS_ENABLED == 1
        #if _MSC_VER
            #include <intrin.h>
            #define V_DEBUG_BREAK() __debugbreak()
        #else
            #define V_DEBUG_BREAK() __builtin_trap()
        #endif

        V_API void report_assertion_failure(const char* expression, const char* message, const char* file, uint32_t line);

        #define V_ASSERT(expr)                                                      \
            {                                                                       \
                if (expr) {                                                         \
                    ;                                                               \
                }                                                                   \
                else {                                                              \
                    report_assertion_failure(#expr, "", __FILE__, __LINE__);        \
                    V_DEBUG_BREAK();                                                \
                }                                                                   \
            }

        #define V_ASSERT_MSG(expr, msg)                                             \
            {                                                                       \
                if (expr) {                                                         \
                    ;                                                               \
                }                                                                   \
                else {                                                              \
                    report_assertion_failure(#expr, msg, __FILE__, __LINE__);       \
                    V_DEBUG_BREAK();                                                \
                }                                                                   \
            }
        
        #ifdef _DEBUG
            #define V_ASSERT_DEBUG(expr) V_ASSERT(expr)
            #define V_ASSERT_MSG_DEBUG(expr, msg) V_ASSERT_MSG(expr, msg)
        #else
            #define V_ASSERT_DEBUG(expr)
            #define V_ASSERT_MSG_DEBUG(expr, msg)
        #endif

    #else
        #define V_ASSERT(expr)
        #define V_ASSERT_MSG(expr, msg)
        #define V_ASSERT_DEBUG(expr)
        #define V_ASSERT_MSG_DEBUG(expr, msg)
    #endif 

#endif // _ASSERTS_H_