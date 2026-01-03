#include "v_memory.h"

#include "core/logger.h"
#include "platform/platform.h"

// TODO: Custom string lib
#include <string.h>

#include <stdio.h>


typedef struct memory_stats_t {
    uint64_t total_allocated;
    uint64_t tagged_allocations[MEMORY_TAG_MAX_TAGS];
} memory_stats_t;


static const char* memory_tag_strings[MEMORY_TAG_MAX_TAGS] = {
    "UNKNOWN          ",
    "ARRAY            ",
    "DARRAY           ",
    "DICT             ",
    "RING_QUEUE       ",
    "BST              ",
    "STRING           ",
    "APPLICATION      ",
    "JOB              ",
    "TEXTURE          ",
    "MATERIAL_INSTANCE",
    "RENDERER         ",
    "GAME             ",
    "TRNSFORM         ",
    "ENTITY           ",
    "ENTITY_NODE      ",
    "SCENE            "
};

static memory_stats_t memory_stats = {0};


void initialize_memory() {
    platform_zero_memory(&memory_stats, sizeof(memory_stats));
}


void shutdown_memory() {
    ;
}


void* v_allocate(uint64_t size, memory_tag_t tag) {
    if (tag == MEMORY_TAG_UNKNOWN) {
        V_LOG_WARN("v_allocate called with MEMORY_TAG_UNKNOWN. Re-classify the allocation.");
    }

    memory_stats.total_allocated += size;
    memory_stats.tagged_allocations[tag] += size;

    // TODO: Memory alignment
    void* block = platform_allocate(size, FALSE);
    platform_zero_memory(block, size);
    return block;
}


void v_free(void* block, uint64_t size, memory_tag_t tag) {
    if (tag == MEMORY_TAG_UNKNOWN) {
        V_LOG_WARN("v_free called with MEMORY_TAG_UNKNOWN. Re-classify the allocation.");
    }

    memory_stats.total_allocated -= size;
    memory_stats.tagged_allocations[tag] -= size;
    platform_free(block, FALSE);
}


void* v_zero_memory(void* block, uint64_t size) {
    return platform_zero_memory(block, size);
}


void* v_copy_memory(void* dest, const void* src, uint64_t size) {
    return platform_copy_memory(dest, src, size);
}


void* v_set_memory(void* dest, int32_t value, uint64_t size) {
    return platform_set_memory(dest, value, size);
}


char* v_get_memory_usage_str() {
    const uint64_t kb = 1024;
    const uint64_t mb = kb * 1024;
    const uint64_t gb = mb * 1024;

    char buffer[8000] = "System memory use (tagged):\n";
    uint64_t offset = strlen(buffer);
    
    for (uint32_t i = 0; i < MEMORY_TAG_MAX_TAGS; ++i) {
        char unit[4] = "";
        float32_t amount = 1.0f;

        if (memory_stats.tagged_allocations[i] >= gb) {
            amount = (float32_t)memory_stats.tagged_allocations[i] / (float32_t)gb;
            strcpy(unit, "GB");
        } else if (memory_stats.tagged_allocations[i] >= mb) {
            amount = (float32_t)memory_stats.tagged_allocations[i] / (float32_t)mb;
            strcpy(unit, "MB");
        } else if (memory_stats.tagged_allocations[i] >= kb) {
            amount = (float32_t)memory_stats.tagged_allocations[i] / (float32_t)kb;
            strcpy(unit, "KB");
        } else {
            amount = (float32_t)memory_stats.tagged_allocations[i];
            strcpy(unit, "B");
        }

        int32_t length = snprintf(buffer + offset, 8000,
                 "  %s : %.2f %s\n",
                 memory_tag_strings[i],
                 amount,
                 unit);

        offset += length;
    }

    char*out_string = _strdup(buffer);
    return out_string;
}   