#ifndef _PLATFORM_H_
    #define _PLATFORM_H_

    #include "defines.h"

    typedef struct platform_state_t {
        void* internal_state;
    } platform_state_t;

    V_API bool8_t platform_startup(
        platform_state_t* platform_state,
        const char* application_name,
        int32_t x_pos,
        int32_t y_pos,
        int32_t window_width,
        int32_t window_height
    );
    
    V_API void platform_shutdown(platform_state_t* platform_state);

    V_API bool8_t platform_pump_messages(platform_state_t* platform_state);

    void* platform_allocate(uint64_t size, bool8_t aligned);
    void platform_free(void* ptr, bool8_t aligned);
    void* platform_zero_memory(void* ptr, uint64_t size);
    void* platform_copy_memory(void* dest, const void* src, uint64_t size);
    void* platform_set_memory(void* dest, int32_t value, uint64_t size);

    void platform_console_write(const char* message, uint8_t log_level);
    void platform_console_write_error(const char* message, uint8_t log_level);

    float64_t platform_get_absolute_time(platform_state_t* platform_state);

    void platform_sleep(uint64_t milliseconds);

#endif  // _PLATFORM_H_