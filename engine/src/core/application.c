#include "application.h"
#include "game_types.h"

#include "platform/platform.h"
#include "logger.h"


typedef struct application_state_t {
    game_t* game_instance;
    bool8_t is_initialized;
    bool8_t is_running;
    bool8_t is_suspended;
    platform_state_t platform;
    int32_t width;
    int32_t height;
    float64_t last_time;
} application_state_t;

static application_state_t app_state = {0};

V_API bool8_t application_startup(game_t* game_instance)
{
    if (app_state.is_initialized) {
        V_LOG_ERROR("Application is already initialized.");
        return FALSE;
    }

    app_state.game_instance = game_instance;
    
    // initialize subsystems here
    initialize_logging();


    // TODO: Remove this
    V_LOG_FATAL("This is a fatal message. %f", 3.14);
    V_LOG_ERROR("This is an error message. %f", 2.71);
    V_LOG_WARN("This is a warning message. %f", 1.61);
    V_LOG_INFO("This is an info message. %f", 0.577);
    V_LOG_DEBUG("This is a debug message. %f", 1.414);
    V_LOG_TRACE("This is a trace message. %f", 2.236);


    // Initialize application state
    app_state.is_running = TRUE;
    app_state.is_suspended = FALSE;


    if (!platform_startup(&app_state.platform, 
        game_instance->app_config.application_name, 
        game_instance->app_config.window_x_pos, 
        game_instance->app_config.window_y_pos, 
        game_instance->app_config.window_width, 
        game_instance->app_config.window_height)
    ) {
        return FALSE;
    }

    if (!app_state.game_instance->initialize(game_instance)) {
        V_LOG_FATAL("Game initialization failed.");
        return FALSE;
    }

    app_state.game_instance->on_resize(app_state.game_instance, app_state.width, app_state.height);


    app_state.is_initialized = TRUE;

    return TRUE;
}


V_API bool8_t application_run()
{
    while(app_state.is_running) {
        if (!platform_pump_messages(&app_state.platform)) {
            app_state.is_running = FALSE;
        }

        if (!app_state.is_suspended) {
            if (!app_state.game_instance->update(app_state.game_instance, 0.0)) {
                V_LOG_FATAL("Game update failed.");
                app_state.is_running = FALSE;
                break;
            }

            if (!app_state.game_instance->render(app_state.game_instance, 0.0)) {
                V_LOG_FATAL("Game render failed.");
                app_state.is_running = FALSE;
                break;
            }
        }
    }

    app_state.is_running = FALSE;

    platform_shutdown(&app_state.platform);

    return TRUE;
}

