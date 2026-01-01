#include "game.h"

#include <core/logger.h>

bool8_t game_initialize(game_t* game)
{
    V_LOG_INFO("Game initialized.");
    return TRUE;
}

bool8_t game_update(game_t* game, float32_t delta_time)
{
    V_LOG_INFO("Game updated.");
    return TRUE;
}

bool8_t game_render(game_t* game, float32_t delta_time)
{
    V_LOG_INFO("Game rendered.");
    return TRUE;
}

bool8_t game_on_resize(game_t* game, uint32_t new_width, uint32_t new_height)
{
    V_LOG_INFO("Window resized to %ux%u.", new_width, new_height);
    return TRUE;
}