#include "application.h"
#include "game_types.h"

#include "logger.h"

#include "platform/platform.h"

#include "core/v_memory.h"
#include "core/event.h"
#include "core/input.h"
#include "core/clock.h"

#include "renderer/renderer_frontend.h"


typedef struct application_state_t {
    game_t* game_instance;
    bool8_t is_initialized;
    bool8_t is_running;
    bool8_t is_suspended;
    platform_state_t platform;
    int32_t width;
    int32_t height;
    clock_t clock;
    float64_t last_time;
} application_state_t;

static application_state_t app_state = {0};


// Event handlers
bool8_t application_on_event(uint16_t code, void* sender, void* listener_inst, event_context_t context);
bool8_t application_on_key(uint16_t code, void* sender, void* listener_inst, event_context_t context);


V_API bool8_t application_initialize(game_t* game_instance)
{
    if (app_state.is_initialized) {
        V_LOG_ERROR("Application is already initialized.");
        return FALSE;
    }

    app_state.game_instance = game_instance;
    
    // initialize subsystems here
    logging_initialize();
    input_initialize();


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


    if (!event_initialize()) {
        V_LOG_ERROR("Event system failed initialization. Application cannot continue.");
        return FALSE;
    }


    event_register_listener(EVENT_CODE_APPLICATION_QUIT, NULL, application_on_event);
    event_register_listener(EVENT_CODE_KEY_PRESSED, NULL, application_on_key);
    event_register_listener(EVENT_CODE_KEY_RELEASED, NULL, application_on_key);


    if (!platform_initialize(&app_state.platform, 
        game_instance->app_config.application_name, 
        game_instance->app_config.window_x_pos, 
        game_instance->app_config.window_y_pos, 
        game_instance->app_config.window_width, 
        game_instance->app_config.window_height)
    ) {
        return FALSE;
    }

    if (!renderer_initialize(game_instance->app_config.application_name, &app_state.platform)) {
        V_LOG_FATAL("Failed to initialize renderer. Aborting application.");
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
    clock_start(&app_state.clock);
    clock_update(&app_state.clock);
    app_state.last_time = app_state.clock.elapsed;
    float64_t running_time = 0;
    uint8_t frame_count = 0;
    float64_t target_frame_seconds = 1.0 / 60;

    V_LOG_INFO(v_get_memory_usage_str());

    while(app_state.is_running) {
        if (!platform_pump_messages(&app_state.platform)) {
            app_state.is_running = FALSE;
        }

        if (!app_state.is_suspended) {
            // Update clock and get delta time.
            clock_update(&app_state.clock);
            float64_t current_time = app_state.clock.elapsed;
            float64_t delta_time = current_time - app_state.last_time;
            float64_t frame_start_time = platform_get_absolute_time();


            if (!app_state.game_instance->update(app_state.game_instance, (float32_t)delta_time)) {
                V_LOG_FATAL("Game update failed.");
                app_state.is_running = FALSE;
                break;
            }

            if (!app_state.game_instance->render(app_state.game_instance, (float32_t)delta_time)) {
                V_LOG_FATAL("Game render failed. Shutting down.");
                app_state.is_running = FALSE;
                break;
            }

            render_packet_t packet;
            packet.delta_time = delta_time;
            renderer_draw_frame(&packet);

            // Figure out how long the frame took and if below
            float64_t frame_end_time = platform_get_absolute_time();
            float64_t frame_elapsed_time = frame_end_time - frame_start_time;
            running_time += frame_elapsed_time;
            float64_t remaining_seconds = target_frame_seconds - frame_elapsed_time;

            if (remaining_seconds > 0) {
                uint64_t remaining_ms = remaining_seconds * 1000;

                // If there is time left, give it back to the OS
                bool8_t limit_frames = FALSE;
                if (remaining_ms > 0 && limit_frames) {
                    platform_sleep(remaining_ms - 1);
                }

                frame_count++;
            }

            // NOTE: Input update/state copying should always be handled
            // after any input should be recorded; I.E. before this line.
            // As a safety, input is the last thing to be updated before
            // this frame ends.
            input_update(delta_time);

            app_state.last_time = current_time;
        }
    }

    app_state.is_running = FALSE;

    event_unregister_listener(EVENT_CODE_APPLICATION_QUIT, NULL, application_on_event);
    event_unregister_listener(EVENT_CODE_KEY_PRESSED, NULL, application_on_key);
    event_unregister_listener(EVENT_CODE_KEY_RELEASED, NULL, application_on_key);

    event_shutdown();
    input_shutdown();
    renderer_shutdown();
    platform_shutdown(&app_state.platform);
    logging_shutdown();

    return TRUE;
}

bool8_t application_on_event(uint16_t code, void* sender, void* listener_inst, event_context_t context)
{
    switch (code)
    {
        case EVENT_CODE_APPLICATION_QUIT:
        {
            V_LOG_INFO("EVENT_CODE_APPLICATION_QUIT received. Shutting down.");
            app_state.is_running = FALSE;
            // Block anything else from processing this
            return TRUE;
        }
    }

    return FALSE;
}


bool8_t application_on_key(uint16_t code, void* sender, void* listener_inst, event_context_t context)
{
    uint16_t key_code = context.data.uint16_data[0];

    if (code == EVENT_CODE_KEY_PRESSED) {
        switch (key_code)
        {
            case KEY_ESCAPE:
            {
                // NOTE: Technically firing an event to itself, but there may be other listeners.
                event_context_t data = { 0 };
                event_fire(EVENT_CODE_APPLICATION_QUIT, NULL, data);
                // Block anything else from processing this
                return TRUE;
            }
            case KEY_A:
            {
                V_LOG_DEBUG("Explicit - A key pressed!");
            }
            default:
            {
                V_LOG_DEBUG("'%c' key pressed in window.", key_code);
            }
        }
    }
    else if (code == EVENT_CODE_KEY_RELEASED) {
        switch (key_code)
        {
            case KEY_B:
            {
                V_LOG_DEBUG("Explicit - B key released!");
            }
            default:
            {
                V_LOG_DEBUG("'%c' key released in window.", key_code);
            }
        }
    }

    return FALSE;
}
