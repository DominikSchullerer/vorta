#ifndef _APPLICATION_H_
    #define _APPLICATION_H_

    #include "defines.h"

    struct game_t;

    typedef struct application_config_t {
        const char* application_name;
        int32_t window_x_pos;
        int32_t window_y_pos;
        int32_t window_width;
        int32_t window_height;
    } application_config_t;

    V_API bool8_t application_initialize(struct game_t* game_instance);

    V_API bool8_t application_run();

#endif // _APPLICATION_H_