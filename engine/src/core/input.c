#include "core/input.h"

#include "core/event.h"
#include "core/v_memory.h"
#include "core/logger.h"

typedef struct keyboard_state_t {
    bool8_t keys[KEYS_MAX_KEYS];
} keyboard_state_t;

typedef struct mouse_state_t {
    int16_t x;
    int16_t y;
    uint8_t buttons[BUTTON_MAX_BUTTONS];
} mouse_state_t;

typedef struct input_state_t {
    keyboard_state_t keyboard_current;
    keyboard_state_t keyboard_previous;
    mouse_state_t mouse_current;
    mouse_state_t mouse_previous;
    bool8_t is_initialized;
} input_state_t;


static input_state_t input_state = { 0 };


void input_initialize() {
    v_zero_memory(&input_state, sizeof(input_state_t));
    input_state.is_initialized = TRUE;
    V_LOG_INFO("Input subsystem initialized.");
}

void input_shutdown() {
    // TODO: Add shutdown routines
    input_state.is_initialized = FALSE;
}

void input_update(float64_t delta_time) {
    if (!input_state.is_initialized) {
        return;
    }

    // Copy current states to previous_states.
    v_copy_memory(&input_state.keyboard_previous, &input_state.keyboard_current, sizeof(keyboard_state_t));
    v_copy_memory(&input_state.mouse_previous, &input_state.mouse_previous, sizeof(mouse_state_t));
}

void input_process_key(keys_t key, bool8_t pressed) {
    if (input_state.keyboard_current.keys[key] != pressed) {
        // Update the internal state.
        input_state.keyboard_current.keys[key] = pressed;

        // Fire off an event for immediate processing
        event_context_t context;
        context.data.uint16_data[0] = key;
        event_fire(pressed ? EVENT_CODE_KEYBOARD_KEY_PRESSED : EVENT_CODE_KEYBOARD_KEY_RELEASED, NULL, context);
    }
}

void input_process_button(buttons_t button, bool8_t pressed) {
    if (input_state.mouse_current.buttons[button] != pressed) {
        // Update the internal state.
        input_state.mouse_current.buttons[button] = pressed;

        // Fire off an event for immediate processing
        event_context_t context;
        context.data.uint16_data[0] = button;
        event_fire(pressed ? EVENT_CODE_MOUSE_BUTTON_PRESSED : EVENT_CODE_MOUSE_BUTTON_RELEASED, NULL, context);
    }
}

void input_process_mouse_move(int16_t x, int16_t y) {
    if (input_state.mouse_current.x != x || input_state.mouse_current.y != y) {
        // Update the internal state.
        input_state.mouse_current.x = x;
        input_state.mouse_current.y = y;

        // Fire off an event for immediate processing
        event_context_t context;
        context.data.uint16_data[0] = x;
        context.data.uint16_data[1] = y;
        event_fire(EVENT_CODE_MOUSE_MOVED, NULL, context);
    }
}

void input_process_mouse_wheel(int8_t z_delta) {
    // Fire off an event for immediate processing
    event_context_t context;
    context.data.uint8_data[0] = z_delta;
    event_fire(EVENT_CODE_MOUSE_WHEEL, NULL, context);
}


bool8_t input_is_key_down(keys_t key) {
    if(!input_state.is_initialized) {
        return FALSE;
    }
    return input_state.keyboard_current.keys[key] == TRUE;
}

bool8_t input_is_key_up(keys_t key) {
    if(!input_state.is_initialized) {
        return TRUE;
    }
    return input_state.keyboard_current.keys[key] == FALSE;
}

bool8_t input_was_key_down(keys_t key) {
    if(!input_state.is_initialized) {
        return FALSE;
    }
    return input_state.keyboard_previous.keys[key] == TRUE;
}

bool8_t input_was_key_up(keys_t key) {
    if(!input_state.is_initialized) {
        return TRUE;
    }
    return input_state.keyboard_previous.keys[key] == FALSE;
}


bool8_t input_is_button_down(buttons_t button) {
    if(!input_state.is_initialized) {
        return FALSE;
    }
    return input_state.mouse_current.buttons[button] == TRUE;
}

bool8_t input_is_button_up(buttons_t button) {
    if(!input_state.is_initialized) {
        return TRUE;
    }
    return input_state.mouse_current.buttons[button] == FALSE;
}

bool8_t input_was_button_down(buttons_t button) {
    if(!input_state.is_initialized) {
        return FALSE;
    }
    return input_state.mouse_previous.buttons[button] == TRUE;
}

bool8_t input_was_button_up(buttons_t button) {
    if(!input_state.is_initialized) {
        return TRUE;
    }
    return input_state.mouse_previous.buttons[button] == FALSE;
}

V_API void input_get_mouse_position(uint32_t* x, uint32_t* y)
{
    if (!input_state.is_initialized) {
        *x = 0;
        *y = 0;
        return;
    }

    *x = input_state.mouse_current.x;
    *y = input_state.mouse_current.y;
}

V_API void input_get_prev_mouse_position(uint32_t* x, uint32_t* y)
{
    if (!input_state.is_initialized) {
        *x = 0;
        *y = 0;
        return;
    }

    *x = input_state.mouse_previous.x;
    *y = input_state.mouse_previous.y;
}
