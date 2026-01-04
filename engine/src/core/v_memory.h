#ifndef _V_MEMORY_H_
    #define _V_MEMORY_H_

    #include "defines.h"

    typedef enum memory_tag_t {
        // For temporary use. Should be assgned one of the below or have a new tag created.
        MEMORY_TAG_UNKNOWN = 0,

        MEMORY_TAG_ARRAY,
        MEMORY_TAG_DARRAY,
        MEMORY_TAG_DICT,
        MEMORY_TAG_RING_QUEUE, 
        MEMORY_TAG_BST,
        MEMORY_TAG_STRING,
        MEMORY_TAG_APPLICATION,
        MEMORY_TAG_JOB,
        MEMORY_TAG_TEXTURE,
        MEMORY_TAG_MATERIAL_INSTANCE,
        MEMORY_TAG_RENDERER,
        MEMORY_TAG_GAME,
        MEMORY_TAG_TRNSFORM,
        MEMORY_TAG_ENTITY,
        MEMORY_TAG_ENTITY_NODE,
        MEMORY_TAG_SCENE,
        
        // Always keep this as the last tag.
        MEMORY_TAG_MAX_TAGS
    } memory_tag_t;

    V_API void memory_initialize();
    V_API void memory_shutdown();

    V_API void* v_allocate(uint64_t size, memory_tag_t tag);
    V_API void v_free(void* block, uint64_t size, memory_tag_t tag);
    
    V_API void* v_zero_memory(void* block, uint64_t size);
    V_API void* v_copy_memory(void* dest, const void* src, uint64_t size);
    V_API void* v_set_memory(void* dest, int32_t value, uint64_t size);

    V_API char* v_get_memory_usage_str();

#endif // _V_MEMORY_H_