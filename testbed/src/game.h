#ifndef _GAME_H_
    #define _GAME_H_

    #include <defines.h>
    #include <game_types.h>

    typedef struct game_state_t {
        float32_t delta_time;
    } game_state_t;

    bool8_t game_initialize(game_t* game);
    bool8_t game_update(game_t* game, float32_t delta_time);
    bool8_t game_render(game_t* game, float32_t delta_time);
    bool8_t game_on_resize(game_t* game, uint32_t new_width, uint32_t new_height);

#endif // _GAME_H_