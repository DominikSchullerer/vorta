#ifndef _EVENT_H_
    #define _EVENT_H_

    #include "defines.h"

    typedef struct event_context_t {
        // 128 bytes
        union {
            int64_t int64_data[2];
            uint64_t uint64_data[2];
            float64_t float64_data[2];

            int32_t int32_data[4];
            uint32_t uint32_data[4];
            float32_t float32_data[4];

            int16_t int16_data[8];
            uint16_t uint16_data[8];

            int8_t int8_data[16];
            uint8_t uint8_data[16];

            char char_data[16];
        } data;
    } event_context_t;

    typedef  bool8_t (*PFN_on_event)(uint16_t code, void* sender, void* listener_inst, event_context_t data);

    bool8_t event_initialize();
    void event_shutdown();


    /**
     * Register to listen for when events are sent with the provided code. 
     * Events with duplicate listener/callback combos will not be registered again and will return FALSE.
     * @param code The event code to listen for.
     * @param listener_inst A pointer to a listener instance. Can be NULL.
     * @param on_event The callback function pointer to be invoked when the event code is fired.
     * @return TRUE if registration was successful, FALSE otherwise.
     */
    V_API bool8_t event_register_listener(uint16_t code, void* listener_inst, PFN_on_event on_event);


    /**
     * Unregister from listening for when events are sent with the provided code.
     * If no matching registration is found, FALSE is returned.
     * @param code The event code to stop listening for.
     * @param listener_inst A pointer to a listener instance. Can be NULL.
     * @param on_event The callback function pointer to be unregistered.
     * @return TRUE if unregistration was successful, FALSE otherwise.
     */
    V_API bool8_t event_unregister_listener(uint16_t code, void* listener_inst, PFN_on_event on_event);


    /**
     * Fire an event to listeners of the given code.
     * If an event handler returns TRUE, the event is considered handled and will not be sent to any
     * further listeners.
     * @param code The event code to fire.
     * @param sender A pointer to the sender instance. Can be NULL.
     * @param event_context The context data to send with the event.
     * @return TRUE if the event was fired successfully, FALSE otherwise.
     */
    V_API bool8_t event_fire(uint16_t code, void* sender, event_context_t event_context);

    // System internal event codes. Application should use codes beyond 255.
    typedef enum system_event_code_t {
        // Shuts the application down on the next frame.
        EVENT_CODE_APPLICATION_QUIT = 0x01,

        // Keyboard key pressed.
        // Context usage:
        // uint16_t key_code = data.data.uint16_data[0];
        EVENT_CODE_KEYBOARD_KEY_PRESSED = 0x02,

        // Keyboard key released.
        // Context usage:
        // uint16_t key_code = data.data.uint16_data[0];
        EVENT_CODE_KEYBOARD_KEY_RELEASED = 0x03,

        // Mouse button pressed.
        // Context usage:
        // uint16_t button_code = data.data.uint16_data[0];
        EVENT_CODE_MOUSE_BUTTON_PRESSED = 0x04,

        // Mouse button released.
        // Context usage:
        // uint16_t button_code = data.data.uint16_data[0];
        EVENT_CODE_MOUSE_BUTTON_RELEASED = 0x05,

        // Mouse moved.
        // Context usage:
        // uint16_t x_pos = data.data.uint16_data[0];
        // uint16_t y_pos = data.data.uint16_data[1];
        EVENT_CODE_MOUSE_MOVED = 0x06,

        // Mouse wheel used.
        // Context usage:
        // uint8_t z_delta = data.data.uint8_data[0];
        EVENT_CODE_MOUSE_WHEEL = 0x07,

        // Resized / resolution changed from the OS.
        // Context usage:
        // uint16_t width = data.data.uint16_data[0];
        // uint16_t height = data.data.uint16_data[1];
        EVENT_CODE_RESIZED = 0x08,

        MAX_EVENT_CODE = 0xFF
    } system_event_code_t;
    

#endif // _EVENT_H_