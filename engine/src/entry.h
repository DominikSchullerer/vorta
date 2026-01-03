#ifndef _ENTRY_H_
    #define _ENTRY_H_

    #include "core/application.h"
    #include "core/logger.h"
    #include "core/v_memory.h"
    #include "game_types.h"

    extern bool8_t create_game(game_t* out_game);

    //****
    //  Entry Point
    //****
    int main(void)
    {
        initialize_memory();

        game_t game_inst = {0};

        if (!create_game(&game_inst)) {
            V_LOG_FATAL("Failed to create game instance.");
            return -1;
        }

        // Ensure all required function pointers are set
        if (!game_inst.initialize || !game_inst.update || !game_inst.render || !game_inst.on_resize) {
            V_LOG_FATAL("Game instance is missing required function implementations.");
            return -2;
        }

        // Initialization
        if (!application_startup(&game_inst)) {
            V_LOG_FATAL("Failed to start application.");
            return -3;
        }

        // Begin the game loop
        if (!application_run()) {
            V_LOG_FATAL("Application runtime error.");
            return -4;
        }

        shutdown_memory();

        return 0;
    }

#endif  // _ENTRY_H_