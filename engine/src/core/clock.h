#ifndef _CLOCK_H_
    #define _CLOCK_H_

    #include "defines.h"

    typedef struct clock_t
    {
        float64_t start_time;
        float64_t elapsed;
    } clock_t;

    void clock_update(clock_t* clock);

    void clock_start(clock_t* clock);
    
    void clock_stop(clock_t* clock);
    

#endif //_CLOCK_H_