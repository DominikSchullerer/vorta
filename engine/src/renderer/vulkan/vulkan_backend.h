#ifndef _VULKAN_BACKEND_H_
    #define _VULKAN_BACKEND_H_ 

    #include "renderer/renderer_backend.h"

    bool8_t vulkan_renderer_backend_initialize(renderer_backend_t* backend, const char* application_name, struct platform_state_t* plat_state);
    void vulkan_renderer_backend_shutdown(renderer_backend_t* backend);

    void vulkan_renderer_backend_on_resized(renderer_backend_t* backend, uint16_t width, uint16_t height);

    bool8_t vulkan_renderer_backend_begin_frame(renderer_backend_t* backend, float32_t delta_time);
    bool8_t vulkan_renderer_backend_end_frame(renderer_backend_t* backend, float32_t delta_time);

#endif // _VULKAN_BACKEND_H_