#pragma once
#include <cstdint>
#include <stdexcept>
#include <iostream>

namespace Platform
{
    //enum class KeyboardKey
    //{
    //    A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    //    _1, _2, _3, _4, _5, _6, _7, _8, _9, _0,
    //    Escape, LControl, LShift, LAlt, LSuper, RSuper, Menu, Lmb, Rmb, Mmb,
    //    Enter, Tab, Backspace, Space, CapsLock, NumLock, ScrollLock, Pause, PrintScreen,
    //    PageUp, PageDown, End, Home, Insert, Delete, Left, Right, Up, Down,
    //    Numpad0, Numpad1, Numpad2, Numpad3, Numpad4, Numpad5, Numpad6, Numpad7, Numpad8, Numpad9,
    //    Add, Subtract, Multiply, Divide, Decimal, NumpadEnter, NumpadEquals,
    //    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
    //    Tilda, Minus, Equals, LBracket, RBracket, Backslash, Semicolon, Apostrophe, Comma, Period, Slash,
    //    Unknown,
    //    Num
    //};

#ifdef _WIN32
    enum class KeyboardKey
    {
        // Alphabet keys (using character values which match VK codes)
        A = 0x41, B = 0x42, C = 0x43, D = 0x44, E = 0x45,
        F = 0x46, G = 0x47, H = 0x48, I = 0x49, J = 0x4A,
        K = 0x4B, L = 0x4C, M = 0x4D, N = 0x4E, O = 0x4F,
        P = 0x50, Q = 0x51, R = 0x52, S = 0x53, T = 0x54,
        U = 0x55, V = 0x56, W = 0x57, X = 0x58, Y = 0x59, Z = 0x5A,

        // Number keys (top row)
        _0 = 0x30, _1 = 0x31, _2 = 0x32, _3 = 0x33, _4 = 0x34,
        _5 = 0x35, _6 = 0x36, _7 = 0x37, _8 = 0x38, _9 = 0x39,

        // Function keys
        F1 = 0x70, F2 = 0x71, F3 = 0x72, F4 = 0x73,
        F5 = 0x74, F6 = 0x75, F7 = 0x76, F8 = 0x77,
        F9 = 0x78, F10 = 0x79, F11 = 0x7A, F12 = 0x7B,

        // Modifier keys
        Escape = 0x1B,
        LControl = 0xA2,
        RControl = 0xA3,
        LShift = 0xA0,
        RShift = 0xA1,
        LAlt = 0xA4,      // VK_LMENU
        RAlt = 0xA5,      // VK_RMENU
        LSuper = 0x5B,    // VK_LWIN
        RSuper = 0x5C,    // VK_RWIN
        Menu = 0x5D,      // VK_APPS (Application key)

        // Navigation keys
        Enter = 0x0D,
        Tab = 0x09,
        Backspace = 0x08,
        Space = 0x20,
        CapsLock = 0x14,
        NumLock = 0x90,
        ScrollLock = 0x91,
        Pause = 0x13,
        PrintScreen = 0x2C,

        // Cursor keys
        PageUp = 0x21,    // VK_PRIOR
        PageDown = 0x22,  // VK_NEXT
        End = 0x23,
        Home = 0x24,
        Insert = 0x2D,
        Delete = 0x2E,
        Left = 0x25,
        Right = 0x27,
        Up = 0x26,
        Down = 0x28,

        // Numpad keys
        Numpad0 = 0x60,
        Numpad1 = 0x61,
        Numpad2 = 0x62,
        Numpad3 = 0x63,
        Numpad4 = 0x64,
        Numpad5 = 0x65,
        Numpad6 = 0x66,
        Numpad7 = 0x67,
        Numpad8 = 0x68,
        Numpad9 = 0x69,
        Add = 0x6B,
        Subtract = 0x6D,
        Multiply = 0x6A,
        Divide = 0x6F,
        Decimal = 0x6E,
        NumpadEnter = 0x0D,  // Same as main Enter
        NumpadEquals = 0x92, // No standard VK, often 0x92

        // Symbol keys (using VK values)
        Tilda = 0xC0,        // VK_OEM_3
        Minus = 0xBD,         // VK_OEM_MINUS
        Equals = 0xBB,        // VK_OEM_PLUS
        LBracket = 0xDB,      // VK_OEM_4
        RBracket = 0xDD,      // VK_OEM_6
        Backslash = 0xDC,     // VK_OEM_5
        Semicolon = 0xBA,     // VK_OEM_1
        Apostrophe = 0xDE,    // VK_OEM_7
        Comma = 0xBC,         // VK_OEM_COMMA
        Period = 0xBE,        // VK_OEM_PERIOD
        Slash = 0xBF,         // VK_OEM_2

        Unknown = 0x00,
        Count
    };
#else
    enum class KeyboardKey
    {
        A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        _1, _2, _3, _4, _5, _6, _7, _8, _9, _0,
        Escape, LControl, LShift, LAlt, LSuper, RSuper, Menu, Lmb, Rmb, Mmb,
        Enter, Tab, Backspace, Space, CapsLock, NumLock, ScrollLock, Pause, PrintScreen,
        PageUp, PageDown, End, Home, Insert, Delete, Left, Right, Up, Down,
        Numpad0, Numpad1, Numpad2, Numpad3, Numpad4, Numpad5, Numpad6, Numpad7, Numpad8, Numpad9,
        Add, Subtract, Multiply, Divide, Decimal, NumpadEnter, NumpadEquals,
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
        Tilda, Minus, Equals, LBracket, RBracket, Backslash, Semicolon, Apostrophe, Comma, Period, Slash,
        Unknown,
        Num
    };
#endif

    enum class MouseButton
    {
        Lmb,            // Left mouse button
        Rmb,            // Right mouse button
        Mmb,            // Middle mouse button
        Button4,       // Usually Browser Back button
        Button5,       // Usually Browser Forward button
        Button6,       // Additional buttons some mice have
        Button7,
        Button8,
        Unknown,
        Count
    };

    enum class MouseButtonState
    {
        Pressed,
        Changed,
        Count
    };

    enum class KeyboardKeyState
    {
        Pressed,
        Released,
        Repeated,
        Count
    };

    enum class IOEvents
    {
        KeyPressed,
        KeyReleased,
        KeyRepeated,
        CharInput,

        MouseButtonPressed,
        MouseLeftButtonPressed,
        MouseRightButtonPressed,
        MouseMiddleButtonPressed,
        MouseExtraButtonPressed,

        MouseButtonReleased,
        MouseLeftButtonReleased,
        MouseRightButtonReleased,
        MouseMiddleButtonReleased,
        MouseExtraButtonReleased,

        MouseMoved,
        MouseScrolled,
        MouseEntered,
        MouseLeft,
        Count
    };

    struct IOEventPolicy {
        using Type = IOEvents;

        template<Type T>
        struct Trait {};

        template <Type T>
        static void handleError(...) {
            std::cerr << "[Platform] Exception in IO event callback: " << Trait<T>::s_name << std::endl;
        }
    };

    template<>
    struct IOEventPolicy::Trait<IOEvents::KeyPressed> {
        using Signature = void(KeyboardKey key, uint32_t scanCode);
        static inline const std::string s_name = "KeyPressed";
    };

    template<>
    struct IOEventPolicy::Trait<IOEvents::KeyReleased> {
        using Signature = void(KeyboardKey key, uint32_t scanCode);
        static inline const std::string s_name = "KeyReleased";
    };

    template<>
    struct IOEventPolicy::Trait<IOEvents::KeyRepeated> {
        using Signature = void(KeyboardKey key, uint32_t scanCode);
        static inline const std::string s_name = "KeyRepeated";
    };

    template<>
    struct IOEventPolicy::Trait<IOEvents::CharInput> {
        using Signature = void(unsigned int codepoint, bool isExtendedKey, bool isAltPressed, bool wasDownBefore);
        static inline const std::string s_name = "CharInput";
    };

    template<>
    struct IOEventPolicy::Trait<IOEvents::MouseButtonPressed> {
        using Signature = void(MouseButton button, int32_t x, int32_t y);
        static inline const std::string s_name = "MouseButtonPressed";
    };

    template<>
    struct IOEventPolicy::Trait<IOEvents::MouseLeftButtonPressed> {
        using Signature = void(int32_t x, int32_t y);
        static inline const std::string s_name = "MouseLeftButtonPressed";
    };

    template<>
    struct IOEventPolicy::Trait<IOEvents::MouseRightButtonPressed> {
        using Signature = void(int32_t x, int32_t y);
        static inline const std::string s_name = "MouseRightButtonPressed";
    };

    template<>
    struct IOEventPolicy::Trait<IOEvents::MouseMiddleButtonPressed> {
        using Signature = void(int32_t x, int32_t y);
        static inline const std::string s_name = "MouseMiddleButtonPressed";
    };

    template<>
    struct IOEventPolicy::Trait<IOEvents::MouseExtraButtonPressed> {
        using Signature = void(MouseButton button, int32_t x, int32_t y);
        static inline const std::string s_name = "MouseExtraButtonPressed";
    };

    template<>
    struct IOEventPolicy::Trait<IOEvents::MouseButtonReleased> {
        using Signature = void(MouseButton button, int32_t x, int32_t y);
        static inline const std::string s_name = "MouseButtonReleased";
    };

    template<>
    struct IOEventPolicy::Trait<IOEvents::MouseLeftButtonReleased> {
        using Signature = void(int32_t x, int32_t y);
        static inline const std::string s_name = "MouseLeftButtonReleased";
    };

    template<>
    struct IOEventPolicy::Trait<IOEvents::MouseRightButtonReleased> {
        using Signature = void(int32_t x, int32_t y);
        static inline const std::string s_name = "MouseRightButtonReleased";
    };

    template<>
    struct IOEventPolicy::Trait<IOEvents::MouseMiddleButtonReleased> {
        using Signature = void(int32_t x, int32_t y);
        static inline const std::string s_name = "MouseMiddleButtonReleased";
    };

    template<>
    struct IOEventPolicy::Trait<IOEvents::MouseExtraButtonReleased> {
        using Signature = void(MouseButton button, int32_t x, int32_t y);
        static inline const std::string s_name = "MouseExtraButtonReleased";
    };

    template<>
    struct IOEventPolicy::Trait<IOEvents::MouseMoved> {
        using Signature = void(int32_t x, int32_t y);
        static inline const std::string s_name = "MouseMoved";
    };

    template<>
    struct IOEventPolicy::Trait<IOEvents::MouseScrolled> {
        using Signature = void(int32_t xoffset, int32_t yoffset);
        static inline const std::string s_name = "MouseScrolled";
    };

    template<>
    struct IOEventPolicy::Trait<IOEvents::MouseEntered> {
        using Signature = void(int32_t x, int32_t y);
        static inline const std::string s_name = "MouseEntered";
    };

    template<>
    struct IOEventPolicy::Trait<IOEvents::MouseLeft> {
        using Signature = void();
        static inline const std::string s_name = "MouseLeft";
    };
}

