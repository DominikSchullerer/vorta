#include "core/clock.h"

#include "platform/platform.h"


void clock_update(clock_t* clock)
{
    if (clock->start_time != 0) {
        clock->elapsed = platform_get_absolute_time() - clock->start_time;
    }
}

void clock_start(clock_t* clock)
{
    clock->start_time = platform_get_absolute_time();
    clock->elapsed = 0;
}

void clock_stop(clock_t* clock)
{
    clock->start_time = 0;
}