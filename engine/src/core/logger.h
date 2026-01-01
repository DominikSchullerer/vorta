#ifndef _LOGGER_H_
    #define _LOGGER_H_

    #include "defines.h"


    #define LOG_WARN_ENBALED 1
    #define LOG_INFO_ENABLED 1

    #if VRELEASE == 1
        #define LOG_DEBUG_ENABLED 0
        #define LOG_TRACE_ENABLED 0
    #else
        #define LOG_DEBUG_ENABLED 1
        #define LOG_TRACE_ENABLED 1
    #endif


    typedef enum log_level_t {
        LOG_LEVEL_FATAL,
        LOG_LEVEL_ERROR,
        LOG_LEVEL_WARNING,
        LOG_LEVEL_INFO,
        LOG_LEVEL_DEBUG,
        LOG_LEVEL_TRACE
    } log_level_t;

    bool8_t initialize_logging();
    void shutdown_logging();

    V_API void log_output(log_level_t level, const char* message, ...);


    #define V_LOG_FATAL(msg, ...) log_output(LOG_LEVEL_FATAL, msg, ##__VA_ARGS__)


    #define V_LOG_ERROR(msg, ...) log_output(LOG_LEVEL_ERROR, msg, ##__VA_ARGS__)


    #if LOG_WARN_ENBALED == 1
        #define V_LOG_WARN(msg, ...) log_output(LOG_LEVEL_WARNING, msg, ##__VA_ARGS__)
    #else
        #define V_LOG_WARN(msg, ...)
    #endif


    #if LOG_INFO_ENABLED == 1 
        #define V_LOG_INFO(msg, ...) log_output(LOG_LEVEL_INFO, msg, ##__VA_ARGS__)
    #else
        #define V_LOG_INFO(msg, ...)
    #endif


    #if LOG_DEBUG_ENABLED == 1
        #define V_LOG_DEBUG(msg, ...) log_output(LOG_LEVEL_DEBUG, msg, ##__VA_ARGS__)
    #else
        #define V_LOG_DEBUG(msg, ...)
    #endif


    #if LOG_TRACE_ENABLED == 1
        #define V_LOG_TRACE(msg, ...) log_output(LOG_LEVEL_TRACE, msg, ##__VA_ARGS__)
    #else
        #define V_LOG_TRACE(msg, ...)
    #endif


#endif // _LOGGER_H_