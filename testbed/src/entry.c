#include "game.h"

#include <entry.h>
#include <platform/platform.h>

// Define the function to create the game instance
bool8_t create_game(game_t* out_game)
{
    out_game->app_config.application_name = "Vorta Test Game";
    out_game->app_config.window_x_pos = 100;
    out_game->app_config.window_y_pos = 100;
    out_game->app_config.window_width = 800;
    out_game->app_config.window_height = 600;

    out_game->initialize = game_initialize;
    out_game->update = game_update;
    out_game->render = game_render;
    out_game->on_resize = game_on_resize;

    out_game->state = platform_allocate(sizeof(game_state_t), FALSE);

    return TRUE;
}


