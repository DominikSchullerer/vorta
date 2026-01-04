#include "platform.h"

#ifdef V_PLATFORM_WINDOWS

    #include "core/logger.h"
    #include "core/input.h"

    #include <windows.h>
    #include <windowsx.h>

    #include <stdlib.h>

    typedef struct internal_state_t {
        HINSTANCE instance;
        HWND window_handle;
        bool8_t running;
    } internal_state_t;
    
    float64_t clock_frequency;
    LARGE_INTEGER start_time;

    LRESULT CALLBACK win32_process_message(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


    bool8_t platform_initialize(
        platform_state_t* platform_state,
        const char* application_name,
        int32_t x_pos,
        int32_t y_pos,
        int32_t width,
        int32_t height
    )
    {
        platform_state->internal_state = malloc(sizeof(internal_state_t));
        internal_state_t* internal_state = (internal_state_t*)platform_state->internal_state;

        internal_state->instance = GetModuleHandleA(NULL);

        // Setup and register window class
        HICON icon = LoadIconA(NULL, IDI_APPLICATION);
        WNDCLASSA window_class = {0};
        window_class.style = CS_DBLCLKS;
        window_class.lpfnWndProc = win32_process_message;
        window_class.cbClsExtra = 0;
        window_class.cbWndExtra = 0;
        window_class.hInstance = internal_state->instance;
        window_class.hIcon = icon;
        window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
        window_class.hbrBackground = NULL;
        window_class.lpszClassName = "vorta_window_class";

        if (!RegisterClassA(&window_class)) {
            MessageBoxA(NULL, "Failed to register window class.", "Error", MB_OK | MB_ICONERROR);
            V_LOG_FATAL("Failed to register window class.");
            return FALSE;
        }

        // Create the window
        uint32_t window_style = WS_OVERLAPPEDWINDOW | WS_SYSMENU | WS_CAPTION;
        uint32_t window_style_ex = WS_EX_APPWINDOW;

        RECT border_rect = {0, 0, width, height};
        AdjustWindowRectEx(&border_rect, window_style, FALSE, window_style_ex);
        
        uint32_t window_x = border_rect.left + x_pos;
        uint32_t window_y = border_rect.top + y_pos;
        uint32_t window_width = border_rect.right - border_rect.left;
        uint32_t window_height = border_rect.bottom - border_rect.top;
        
        HWND handle = CreateWindowExA(
            window_style_ex,
            "vorta_window_class",
            application_name,
            window_style,
            window_x,
            window_y,
            window_width,
            window_height,
            NULL,
            NULL,
            internal_state->instance,
            NULL
        );

        if (!handle) {
            MessageBoxA(NULL, "Failed to create window.", "Error", MB_OK | MB_ICONERROR);
            V_LOG_FATAL("Failed to create window.");
            return FALSE;
        }
        internal_state->window_handle = handle;

        // Show the window
        bool32_t should_activate = TRUE;
        int32_t show_window_command_flags = should_activate ? SW_SHOW : SW_SHOWNOACTIVATE;

        ShowWindow(internal_state->window_handle, show_window_command_flags);

        // Clock setup
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        clock_frequency = 1.0 / (float64_t)frequency.QuadPart;
        QueryPerformanceCounter(&start_time);

        return TRUE;
    }


    void platform_shutdown(platform_state_t* platform_state)
    {
        internal_state_t* internal_state = (internal_state_t*)platform_state->internal_state;

        if (internal_state->window_handle) {
            DestroyWindow(internal_state->window_handle);
            internal_state->window_handle = NULL;
        }

        if (internal_state->instance) {
            // Unload icon if needed
            UnregisterClassA("vorta_window_class", internal_state->instance);
        }   

        free(internal_state);
        platform_state->internal_state = NULL;
    }


    bool8_t platform_pump_messages(platform_state_t* platform_state)
    {
        MSG message;
        while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }

        return TRUE;
    }

    void* platform_allocate(uint64_t size, bool8_t aligned)
    {
        return malloc(size);
    }

    void platform_free(void* block, bool8_t aligned)
    {
        free(block);
    }

    void* platform_zero_memory(void* block, uint64_t size)
    {
        return memset(block, 0, size);
    }

    void* platform_copy_memory(void* dest, const void* src, uint64_t size)
    {
        return memcpy(dest, src, size);
    }

    void* platform_set_memory(void* dest, int32_t value, uint64_t size)
    {
        return memset(dest, value, size);
    }

    void platform_console_write(const char* message, uint8_t log_level)
    {
        OutputDebugStringA(message);
        
        HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        uint64_t length = strlen(message);
        DWORD number_written = 0;
        
        // FATAL=0, ERROR=1, WARN=2, INFO=3, DEBUG=4, TRACE=5
        static uint8_t colour_codes[] = {64, 4, 6, 2, 1, 8};
        SetConsoleTextAttribute(console_handle, colour_codes[log_level]);
        
        WriteConsoleA(console_handle, message, (DWORD)length, &number_written, NULL);
    }

    void platform_console_write_error(const char* message, uint8_t log_level)
    {
        OutputDebugStringA(message);
        
        HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);
        uint64_t length = strlen(message);
        DWORD number_written = 0;
        
        // FATAL=0, ERROR=1, WARN=2, INFO=3, DEBUG=4, TRACE=5
        static uint8_t colour_codes[] = {64, 4, 6, 2, 1, 8};
        SetConsoleTextAttribute(console_handle, colour_codes[log_level]);
        
        WriteConsoleA(console_handle, message, (DWORD)length, &number_written, NULL);
    }

    float64_t platform_get_absolute_time()
    {
        LARGE_INTEGER now_time;
        QueryPerformanceCounter(&now_time);
        return (float64_t)now_time.QuadPart * clock_frequency;
    }

    void platform_sleep(uint64_t milliseconds)
    {
        Sleep(milliseconds);
    }

    
    LRESULT CALLBACK win32_process_message(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg) {
            case WM_ERASEBKGND:
            {
                // Notify the OS that erasing will be handled by the application to prevent flickering.
                return 1;
            }
            case WM_CLOSE:
            {
                // TODO: Fire an event for quitting.
                return 0;
            }
            case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            }
            case WM_SIZE:
            {
                // RECT rect;
                // GetClientRect(hwnd, &rect);
                // int32_t width = rect.right - rect.left;
                // int32_t height = rect.bottom - rect.top;

                // TODO: Fire an event for window resize.
                break;
            }
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
            case WM_KEYUP:
            case WM_SYSKEYUP:
            {
                bool8_t pressed = (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN);
                keys_t key = (uint16_t)wParam;
                input_process_key(key, pressed);
                break;
            }
            case WM_MOUSEMOVE:
            {
                int32_t mouse_x = GET_X_LPARAM(lParam);
                int32_t mouse_y = GET_Y_LPARAM(lParam);
                input_process_mouse_move(mouse_x, mouse_y);
                break;
            }
            case WM_MOUSEWHEEL:
            {
                int32_t z_delta = GET_WHEEL_DELTA_WPARAM(wParam);
                if (z_delta != 0) {
                    // Flatten the input to -1 or 1
                    z_delta = (z_delta < 0) ? -1 : 1;
                }
                input_process_mouse_wheel(z_delta);
                break;
            }
            case WM_LBUTTONDOWN:
            case WM_LBUTTONUP:
            {
                bool8_t pressed = uMsg == WM_LBUTTONDOWN;
                input_process_button(BUTTON_LEFT, pressed);
                break;
            }
            case WM_RBUTTONDOWN: 
            case WM_RBUTTONUP:
            {
                bool8_t pressed = uMsg == WM_RBUTTONDOWN;
                input_process_button(BUTTON_RIGHT, pressed);
                break;
            }
            case WM_MBUTTONDOWN:
            case WM_MBUTTONUP:
            {
                bool8_t pressed = uMsg == WM_MBUTTONDOWN;
                input_process_button(BUTTON_MIDDLE, pressed);
                break;
            }
        }

        return DefWindowProcA(hwnd, uMsg, wParam, lParam);
    }

#endif  // V_PLATFORM_WINDOWS