#include "vulkan_backend.h"

#include "vulkan_types.inl"

#include "core/logger.h"

static vulkan_context_t context = { 0 };

bool8_t vulkan_renderer_backend_initialize(renderer_backend_t* backend, const char* application_name, struct platform_state_t* plat_state)
{
    // Setup Vulkan instance
    VkApplicationInfo app_info = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
    app_info.apiVersion = VK_API_VERSION_1_2;
    app_info.pApplicationName = application_name;
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "Vorta Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    
    VkInstanceCreateInfo create_info = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    create_info.pApplicationInfo = &app_info;
    create_info.enabledExtensionCount = 0;
    create_info.ppEnabledExtensionNames = 0;
    create_info.enabledLayerCount = 0;
    create_info.ppEnabledLayerNames = 0;
    
    // TODO: custom allocator
    context.allocator = NULL;
    VkResult result = vkCreateInstance(&create_info, context.allocator, &context.instance);
    if (result != VK_SUCCESS) {
        V_LOG_ERROR("vkCreateInstance failed with result: %d", result);
        return FALSE;
    }
    
    V_LOG_INFO("Vulkan renderer initialized successfully.");
    return TRUE;
}

void vulkan_renderer_backend_shutdown(renderer_backend_t* backend)
{
    ;
}

void vulkan_renderer_backend_on_resized(renderer_backend_t* backend, uint16_t width, uint16_t height)
{
    ;
}

bool8_t vulkan_renderer_backend_begin_frame(renderer_backend_t* backend, float32_t delta_time)
{
    return TRUE;
}

bool8_t vulkan_renderer_backend_end_frame(renderer_backend_t* backend, float32_t delta_time)
{
    return TRUE;
}
