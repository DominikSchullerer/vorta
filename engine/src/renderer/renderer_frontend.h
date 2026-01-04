#ifndef _RENDERER_FRONTEND_H_
    #define _RENDERER_FRONTEND_H_

    #include "renderer_types.inl"

    struct static_mesh_data;
    struct platform_state;

    bool8_t renderer_initialize(const char* application_name, struct platform_state_t* plat_state);
    void renderer_shutdown();

    void renderer_on_resized(uint16_t width, uint16_t height);

    bool8_t renderer_draw_frame(render_packet_t* packet);

#endif // _RENDERER_FRONTEND_H_