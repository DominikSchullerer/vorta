#include <core/logger.h>
#include <core/asserts.h>

// TODO: Test
#include <platform/platform.h>

int main(void)
{
    V_LOG_FATAL("This is a fatal message. %f", 3.14);
    V_LOG_ERROR("This is an error message. %f", 2.71);
    V_LOG_WARN("This is a warning message. %f", 1.61);
    V_LOG_INFO("This is an info message. %f", 0.577);
    V_LOG_DEBUG("This is a debug message. %f", 1.414);
    V_LOG_TRACE("This is a trace message. %f", 2.236);

    platform_state_t platform_state = {0};

    if (platform_startup(&platform_state, "Test Application", 100, 100, 800, 600)) {
        while(TRUE) {
            platform_pump_messages(&platform_state);
        }
    }

    platform_shutdown(&platform_state);

    return 0;
}