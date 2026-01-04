#include "renderer_backend.h"

#include "vulkan/vulkan_backend.h"

bool8_t renderer_backend_create(renderer_backend_type_t type, struct platform_state_t* platform_state, renderer_backend_t* out_renderer_backend)
{
    out_renderer_backend->platform_state = platform_state;

    if(type == RENDERER_BACKEND_TYPE_VULKAN) {
        out_renderer_backend->initialize = vulkan_renderer_backend_initialize;
        out_renderer_backend->shutdown = vulkan_renderer_backend_shutdown;
        out_renderer_backend->begin_frame = vulkan_renderer_backend_begin_frame;
        out_renderer_backend->end_frame = vulkan_renderer_backend_end_frame;
        out_renderer_backend->resized = vulkan_renderer_backend_on_resized;

        return TRUE;
    }

    return FALSE;
}


void renderer_backend_destroy(renderer_backend_t renderer_backend)
{
    renderer_backend.initialize = NULL;
    renderer_backend.shutdown = NULL;
    renderer_backend.begin_frame = NULL;
    renderer_backend.end_frame = NULL;
    renderer_backend.resized = NULL;
}