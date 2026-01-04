#ifndef _RENDERER_BACKEND_H_
    #define _RENDERER_BACKEND_H_

    #include "renderer_types.inl"

    struct platform_state;

    bool8_t renderer_backend_create(renderer_backend_type_t type, struct platform_state_t* plat_state, renderer_backend_t* out_renderer_backend);
    void renderer_backend_destroy(renderer_backend_t renderer_backend);

#endif // _RENDERER_BACKEND_H_