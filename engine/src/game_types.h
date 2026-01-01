#ifndef _GAME_TYPES_H_
    #define _GAME_TYPES_H_

    #include "core/application.h"

    typedef struct game_t game_t;

    struct game_t {
        application_config_t app_config;

        bool8_t (*initialize)(game_t* game);
        bool8_t (*update)(game_t* game, float32_t delta_time);
        bool8_t (*render)(game_t* game, float32_t delta_time);
        bool8_t (*on_resize)(game_t* game, uint32_t new_width, uint32_t new_height);
        
        void* state;
    };

#endif // _GAME_TYPES_H_