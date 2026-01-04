#include "event.h"

#include "core/v_memory.h"
#include "core/logger.h"
#include "containers/darray.h"


typedef struct registered_event_t {
    void* listener;
    PFN_on_event callback;
} registered_event_t;

typedef struct event_code_entry_t {
    registered_event_t* events;
} event_code_entry_t;


#define MAX_MESSAGE_CODES 16384
typedef struct event_system_state_t {
    event_code_entry_t registered[MAX_MESSAGE_CODES];
    bool8_t is_initialized;
} event_system_state_t;


static event_system_state_t event_system = { 0 };


bool8_t event_initialize() 
{
    if (event_system.is_initialized) {
        return FALSE;
    }

    v_zero_memory(&event_system, sizeof(event_system));
    
    event_system.is_initialized = TRUE;
    return TRUE;
}


void event_shutdown() 
{
    // Free the events arrays. Any objects pointed to should be destroyed on their own.
    for (uint16_t i = 0; i < MAX_MESSAGE_CODES; i++) {
        if (event_system.registered[i].events != NULL) {
            darray_destroy(event_system.registered[i].events);
            event_system.registered[i].events = NULL;
        }
    }
    event_system.is_initialized = FALSE;
}


bool8_t event_register_listener(uint16_t code, void* listener, PFN_on_event on_event)
{
    if (!event_system.is_initialized) {
        return FALSE;
    }

    if (event_system.registered[code].events == NULL) {
        event_system.registered[code].events = darray_create(registered_event_t);
    }

    uint64_t registered_count = darray_length(event_system.registered[code].events);
    for (uint64_t i = 0; i < registered_count; i++) {
        registered_event_t reg_event = event_system.registered[code].events[i];
        if (reg_event.listener == listener && reg_event.callback == on_event) {
            V_LOG_WARN("Could not register event listener. Listener is already registered.");
            return FALSE;
        }
    }

    // If at this point, no duplicate was found. Proceed with registration.
    registered_event_t event;
    event.listener = listener;
    event.callback = on_event;
    darray_push(event_system.registered[code].events, event);
    
    return TRUE;
}


bool8_t event_unregister_listener(uint16_t code, void* listener, PFN_on_event on_event)
{
    if (!event_system.is_initialized) {
        return FALSE;
    }

    if (event_system.registered[code].events == NULL) {
        V_LOG_WARN("Could not unregister event listener. No listeners registered for code %d.", code);
        return FALSE;
    }

    uint64_t registered_count = darray_length(event_system.registered[code].events);
    for (uint64_t i = 0; i < registered_count; i++) {
        registered_event_t reg_event = event_system.registered[code].events[i];
        if (reg_event.listener == listener && reg_event.callback == on_event) {
            // Found one, remove it
            registered_event_t popped_event;
            darray_pop_at(event_system.registered[code].events, i, &popped_event);
            return TRUE;
        }
    }

    // Not found
    return FALSE;
}


bool8_t event_fire(uint16_t code, void* sender, event_context_t context) {
    if (!event_system.is_initialized) {
        return FALSE;
    }

    // If nothing is registered for the code, boot out.
    if (event_system.registered[code].events == NULL) {
        return FALSE;
    }

    uint64_t registered_count = darray_length(event_system.registered[code].events);
    for (uint64_t i = 0; i < registered_count; i++) {
        registered_event_t reg_event = event_system.registered[code].events[i];
        if (reg_event.callback(code, sender, reg_event.listener, context)) {
            // Message has been handled, do not send to other listeners
            return TRUE;
        }
    }

    // Not found.
    return FALSE;
}

