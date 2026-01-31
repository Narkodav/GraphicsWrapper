#pragma once
#include <cstdint>
#include <stdexcept>
#include <iostream>

namespace Platform
{
    enum class CursorMode {
        Normal,
        Hidden,
        Disabled  // For raw motion input
    };

    struct Position
    {
        int32_t x, y;
    };

    struct Extent
    {
        int32_t width, height;
    };

    struct WindowAttributes
    {
        enum class Type {
            // Window behavior
            Resizable,                  // bool
            Visible,                    // bool
            Decorated,                  // bool
            Focused,                    // bool
            AutoIconify,                // bool
            Floating,                   // bool
            Maximized,                  // bool
            CenterCursor,               // bool
            TransparentFramebuffer,     // bool
            FocusOnShow,                // bool

            // Input mode
            CursorMode,                 // CursorMode enum
        };

        // Window behavior
        bool resizable = true;                  // GLFW_RESIZABLE
        bool visible = true;                    // GLFW_VISIBLE
        bool decorated = true;                  // GLFW_DECORATED (title bar, borders)
        bool focused = true;                    // GLFW_FOCUSED
        bool autoIconify = true;                // GLFW_AUTO_ICONIFY
        bool floating = false;                  // GLFW_FLOATING (always on top)
        bool maximized = false;                 // GLFW_MAXIMIZED
        bool centerCursor = false;              // GLFW_CENTER_CURSOR
        bool transparentFramebuffer = false;    // GLFW_TRANSPARENT_FRAMEBUFFER
        bool focusOnShow = true;                // GLFW_FOCUS_ON_SHOW

        CursorMode cursorMode = CursorMode::Normal;

        static WindowAttributes defaultAtr() {
            return WindowAttributes();
        }

        static WindowAttributes firstPersonGameMaximisedAtr() {
            WindowAttributes atr;
            atr.centerCursor = true;
            atr.cursorMode = CursorMode::Disabled;
            atr.maximized = true;
            return atr;
        }

        static WindowAttributes firstPersonGameMinimisedAtr() {
            WindowAttributes atr;
            atr.centerCursor = true;
            atr.cursorMode = CursorMode::Disabled;
            return atr;
        }
    };

    enum class WindowEvents
    {
        WindowResized,
        WindowMoved,
        WindowFocused,
        WindowUnfocused,
        WindowMinimized,
        WindowMaximized,
        WindowClosed,
        WindowRefresh,
        WindowContentScaleChanged,

        Native, //called on all events, returns platform native data
        Count
    };

    namespace Win32
    {
        using WindowHandle = void*;
        using InstanceHandle = void*;
        using LongResult = long long;               // matches LongResult (long long on 64-bit)
        using UnsignedInt = unsigned int;           // matches UnsignedInt  
        using WideParameter = unsigned long long;   // matches WideParameter (unsigned long long on 64-bit)
        using LongParameter = long long;            // matches LongParameter (long long on 64-bit)
    }

    struct WindowEventPolicy {
        using Type = WindowEvents;

        template<Type T>
        struct Trait {};

        template <Type T>
        static void handleError(...) {
            std::cerr << "[Platform] Exception in window event callback: " << Trait<T>::s_name << std::endl;
        }

    };

#ifdef _WIN32
    template<>
    struct WindowEventPolicy::Trait<WindowEvents::Native> {
        using Signature = void(Win32::WindowHandle windowHandle, Win32::UnsignedInt unsignedMessage,
            Win32::WideParameter wideParameter, Win32::LongParameter longParameter);
        static constexpr const char* s_name = "Native";
    };
#else


#endif

    template<>
    struct WindowEventPolicy::Trait<WindowEvents::WindowResized> {
        using Signature = void(int width, int height);
        static constexpr const char* s_name = "WindowResized";
    };

    template<>
    struct WindowEventPolicy::Trait<WindowEvents::WindowMoved> {
        using Signature = void(int x, int y);
        static constexpr const char* s_name = "WindowMoved";
    };

    template<>
    struct WindowEventPolicy::Trait<WindowEvents::WindowFocused> {
        using Signature = void();
        static constexpr const char* s_name = "WindowFocused";
    };

    template<>
    struct WindowEventPolicy::Trait<WindowEvents::WindowUnfocused> {
        using Signature = void();
        static constexpr const char* s_name = "WindowUnfocused";
    };

    template<>
    struct WindowEventPolicy::Trait<WindowEvents::WindowMinimized> {
        using Signature = void(int width, int height);
        static constexpr const char* s_name = "WindowMinimized";
    };

    template<>
    struct WindowEventPolicy::Trait<WindowEvents::WindowMaximized> {
        using Signature = void(int width, int height);
        static constexpr const char* s_name = "WindowMaximized";
    };

    template<>
    struct WindowEventPolicy::Trait<WindowEvents::WindowClosed> {
        using Signature = void();
        static constexpr const char* s_name = "WindowClosed";
    };

    template<>
    struct WindowEventPolicy::Trait<WindowEvents::WindowRefresh> {
        using Signature = void();
        static constexpr const char* s_name = "WindowRefresh";
    };

    template<>
    struct WindowEventPolicy::Trait<WindowEvents::WindowContentScaleChanged> {
        using Signature = void(float xscale, float yscale);
        static constexpr const char* s_name = "WindowContentScaleChanged";
    };
}

