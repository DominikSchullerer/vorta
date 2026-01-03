#include "containers/darray.h"

#include "core/v_memory.h"
#include "core/logger.h"


V_API void* _darray_create(uint64_t length, uint64_t stride)
{
    uint64_t header_size = DARRAY_FIELD_LENGTH * sizeof(uint64_t);
    uint64_t array_size = length * stride;
    uint64_t* new_array = v_allocate(header_size + array_size, MEMORY_TAG_DARRAY);
    v_set_memory(new_array, 0, header_size + array_size);
    new_array[DARRAY_CAPACITY] = length;
    new_array[DARRAY_LENGTH] = 0;
    new_array[DARRAY_STRIDE] = stride;
    return (void*)(new_array + DARRAY_FIELD_LENGTH);
}


V_API void _darray_destroy(void* array)
{
    uint64_t* header = (uint64_t*)array - DARRAY_FIELD_LENGTH;
    uint64_t header_size = DARRAY_FIELD_LENGTH * sizeof(uint64_t);
    uint64_t total_size = header_size + header[DARRAY_CAPACITY] + header[DARRAY_STRIDE];
    v_free(header, total_size, MEMORY_TAG_DARRAY);
}


V_API uint64_t _darray_field_get(void* array, uint64_t field)
{
    uint64_t* header = (uint64_t*)array - DARRAY_FIELD_LENGTH;
    return header[field];
}


V_API void _darray_field_set(void* array, uint64_t field, uint64_t value)
{
    uint64_t* header = (uint64_t*)array - DARRAY_FIELD_LENGTH;
    header[field] = value;
}


V_API void* _darray_resize(void* array)
{
    uint64_t length = darray_length(array);
    uint64_t stride = darray_stride(array);
    void* temp = _darray_create(
        (DARRAY_RESIZE_FACTOR * darray_capacity(array)),
        stride
    );

    v_copy_memory(temp, array, length * stride);

    _darray_field_set(temp, DARRAY_LENGTH, length);
    _darray_destroy(array);
    return temp;
}


V_API void* _darray_push(void* array, const void* value_ptr)
{
    uint64_t length = darray_length(array);
    uint64_t stride = darray_stride(array);
    if (length >= darray_capacity(array)) {
        array = _darray_resize(array);
    }

    uint64_t addr = (uint64_t)array;
    addr += (length * stride);
    v_copy_memory((void*)addr, value_ptr, stride);
    _darray_field_set(array, DARRAY_LENGTH, length + 1);
    return array;
}


V_API void _darray_pop(void* array, void* dest)
{
    uint64_t length = darray_length(array);
    uint64_t stride = darray_stride(array);
    uint64_t addr = (uint64_t)array;
    addr += ((length - 1) * stride);
    v_copy_memory(dest, (void*)addr, stride);
    _darray_field_set(array, DARRAY_LENGTH, length - 1);
}

// TODO: v_copy_memory overlaps -> change it
V_API void* _darray_insert_at(void* array, uint64_t index, const void* value_ptr)
{
    uint64_t length = darray_length(array);
    uint64_t stride = darray_stride(array);
    if (index >= length) {
        V_LOG_ERROR("Index outside of the bounds of this array! Length: %d, index: %d", length, index);
        return array;
    }
    
    if (length >= darray_capacity(array)) {
        array = _darray_resize(array);
    }

    uint64_t addr = (uint64_t)array;
    
    // If not on the last element, copy the rest outward.
    if (index != length - 1) {
        v_copy_memory(
            (void*)(addr + ((index + 1) * stride)),
            (void*)(addr + (index * stride)),
            stride * (length - index)
        );
    }

    // Set the value at the index
    v_copy_memory((void*)(addr + (index * stride)), value_ptr, stride);

    _darray_field_set(array, DARRAY_LENGTH, length + 1);
    return array;
}

// TODO: v_copy_memory overlaps -> change it
V_API void _darray_pop_at(void* array, uint64_t index, void* dest)
{
    uint64_t length = darray_length(array);
    uint64_t stride = darray_stride(array);
    if (index >= length) {
        V_LOG_ERROR("Index outside of the bounds of this array! Length: %d, index: %d", length, index);
    }

    uint64_t addr = (uint64_t)array;
    v_copy_memory(dest, (void*)(addr + (index * stride)), stride);

    // If not on the last element, snip out out the entry and copy the rest inward.
    if (index != length -1) {
        v_copy_memory(
            (void*)(addr + (index * stride)),
            (void*)(addr + ((index + 1) * stride)),
            stride * (length - index)
        );
    }

    _darray_field_set(array, DARRAY_LENGTH, length - 1);
}