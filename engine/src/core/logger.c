#include "logger.h"
#include "asserts.h"
#include "platform/platform.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

bool8_t logging_initialize()
{
    // TODO: Implement logging initialization logic here.
    return TRUE;
}

void logging_shutdown()
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

    bool8_t is_error = level <= LOG_LEVEL_ERROR;
    const int32_t msg_length = 32000;
    char out_message[msg_length];
    memset(out_message, 0, sizeof(out_message) * sizeof(*out_message));

    __builtin_va_list arg_ptr;
    va_start(arg_ptr, message);
    vsnprintf(out_message, sizeof(out_message) - 1, message, arg_ptr);
    va_end(arg_ptr);

    char final_message[msg_length + 32];
    sprintf(final_message, "%s%s\n", level_strings[level], out_message);
    
    // platform-specific output handling
    if (is_error) {
        platform_console_write_error(final_message, (uint8_t)level);
    } else {
        platform_console_write(final_message, (uint8_t)level);
    }
}

void report_assertion_failure(const char* expression, const char* message, const char* file, uint32_t line)
{
    log_output(LOG_LEVEL_FATAL, "Assertion failed: (%s), message: '%s', in file: %s, at line: %u", expression, message, file, line);
}
