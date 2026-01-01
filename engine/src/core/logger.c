#include "logger.h"
#include "asserts.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

b8_t initialize_logging()
{
    // TODO: Implement logging initialization logic here.
    return TRUE;
}

void shutdown_logging()
{
    // TODO: Implement logging shutdown logic here.
}

void log_output(log_level_t level, const char* message, ...)
{
    const char* level_strings[] = {
        "[FATAL]: ",
        "[ERROR]: ",
        "[WARN]:  ",
        "[INFO]:  ",
        "[DEBUG]: ",
        "[TRACE]: "
    };

    // b8_t is_error = level <= LOG_LEVEL_ERROR;

    char out_message[32000];
    memset(out_message, 0, sizeof(out_message) * sizeof(*out_message));

    __builtin_va_list arg_ptr;
    va_start(arg_ptr, message);
    vsnprintf(out_message, sizeof(out_message) - 1, message, arg_ptr);
    va_end(arg_ptr);

    char final_message[32032];
    sprintf(final_message, "%s%s\n", level_strings[level], out_message);
    
    // TODO: platform-specific output handling
    printf("%s", final_message);
}

void report_assertion_failure(const char* expression, const char* message, const char* file, uint32_t line)
{
    log_output(LOG_LEVEL_FATAL, "Assertion failed: (%s), message: '%s', in file: %s, at line: %u", expression, message, file, line);
}
