#ifndef _RENDERER_TYPES_INL_
    #define _RENDERER_TYPES_INL_

    #include "defines.h"

    typedef enum renderer_backend_type_t {
        RENDERER_BACKEND_TYPE_VULKAN,
        RENDERER_BACKEND_TYPE_OPENGL,
        RENDERER_BACKEND_TYPE_DIRECTX
    } renderer_backend_type_t;

    typedef struct renderer_backend_t {
        struct platform_state_t* platform_state;
        uint64_t frame_number;

        bool8_t (*initialize)(struct renderer_backend_t* backend, const char* application_name, struct platform_state_t* plat_state);
        
        void (*shutdown)(struct renderer_backend_t* backend);

        void (*resized)(struct renderer_backend_t* backend, uint16_t width, uint16_t height);

        bool8_t (*begin_frame)(struct renderer_backend_t* backend, float32_t delta_time);
        bool8_t (*end_frame)(struct renderer_backend_t* backend, float32_t delta_time);
    } renderer_backend_t;

    typedef struct render_packet_t {
        float32_t delta_time;
    } render_packet_t;

#endif // _RENDERER_TYPES_INL_