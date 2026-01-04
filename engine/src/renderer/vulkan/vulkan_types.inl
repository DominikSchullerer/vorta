#ifndef _VULKAN_TYPES_INL_
    #define _VULKAN_TYPES_INL_

    #include "defines.h"

    #include <vulkan/vulkan.h>

    typedef struct vulkan_context_t {
        VkInstance instance;
        VkAllocationCallbacks* allocator;
    } vulkan_context_t;

#endif // _VULKAN_TYPES_INL_