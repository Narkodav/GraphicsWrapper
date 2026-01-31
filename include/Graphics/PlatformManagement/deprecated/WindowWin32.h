#pragma once
#include <string>

#include "WindowEvents.h"
#include "IOEvents.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "MultiThreading/EventSuperSystem.h"

namespace Platform
{
    using WindowHandle = void*;

    //windows are thread local, declare them thread_local for clarity
    class WindowWin32
    {
    private:
        using LResult = long long;          // matches LResult (long long on 64-bit)
        using HWnd = void*;                 // matches HWnd (opaque pointer)
        using UInt = unsigned int;          // matches UINT  
        using WParam = unsigned long long;  // matches WParam (unsigned long long on 64-bit)
        using LParam = long long;           // matches LParam (long long on 64-bit)
        using WndClass = void*;             // matches ATOM or WNDCLASSEX* (opaque)
        using HInstance = void*;            // matches HInstance
        using HDC = void*;                  // matches HDC
        using HMenu = void*;                // matches HMENU
        using DWord = unsigned long;        // matches DWORD
        using Word = unsigned short;        // matches WORD
        using Bool = int;                   // matches BOOL (actually int, not bool)
        using LPCStr = const char*;         // matches LPCSTR

        static LResult CALLBACK WindowProc(HWnd hwnd, UInt uMsg, WParam wParam, LParam lParam);

    public:
        using EventManager = MT::EventSuperSystem<WindowEventPolicy, IOEventPolicy>;

        using WindowEventSubscription = MT::EventSystem<WindowEventPolicy>::Subscription;
        using IOEventSubscription = MT::EventSystem<IOEventPolicy>::Subscription;
    private:
        HWnd m_hwnd = nullptr;
        EventManager m_windowEvents;
        bool m_mouseInWindow = false;

        thread_local static inline size_t s_windowCounter = 0;

    public:
        WindowWin32() : m_hwnd(nullptr), m_windowEvents(), m_mouseInWindow(false) {};
        WindowWin32(int width, int height, std::wstring_view title);
        ~WindowWin32();

        WindowWin32(WindowWin32&& other) noexcept {
            m_hwnd = std::exchange(other.m_hwnd, nullptr);
            m_windowEvents = std::exchange(other.m_windowEvents, EventManager());
            m_mouseInWindow = std::exchange(other.m_mouseInWindow, false);
            if (m_hwnd) {
                setThisPointer();
            }
        };

        WindowWin32& operator=(WindowWin32&& other) noexcept
        {
            if (this == &other)
                return *this;
            destroy();
            m_hwnd = std::exchange(other.m_hwnd, nullptr);
            m_windowEvents = std::exchange(other.m_windowEvents, EventManager());
            m_mouseInWindow = std::exchange(other.m_mouseInWindow, false);
            if (m_hwnd) {
                setThisPointer();
            }
            return *this;
        };

        WindowWin32(const WindowWin32&) noexcept = delete;
        WindowWin32& operator=(const WindowWin32&) noexcept = delete;

        WindowHandle getHandle() const { return m_hwnd; }

        void pollEvents();
        static void pollEvents();
        bool popEvent();
        static bool popEvent();
        void show();

        void destroy();
        void create(int width, int height, std::wstring_view title);

    private:
        void setThisPointer();
        static WindowWin32* getThisPointer(HWnd hwnd);

        template <LResult(WindowWin32::* func)(WParam, LParam)>
        static inline LResult callEvent(HWnd hwnd, WParam wParam, LParam lParam) {
            WindowWin32* window = getThisPointer(hwnd);
            if (window)
                return (window->*func)(wParam, lParam);
            return 0;
        }

        static HInstance getHInstance();

        static LResult onNull(WParam wParam, LParam lParam);
        static LResult onCreate(WParam wParam, LParam lParam);
        LResult onDestroy(WParam wParam, LParam lParam);

        LResult onWindowResized(WParam wParam, LParam lParam);
        LResult onWindowMoved(WParam wParam, LParam lParam);
        LResult onWindowFocused(WParam wParam, LParam lParam);
        LResult onWindowUnfocused(WParam wParam, LParam lParam);
        LResult onWindowClosed(WParam wParam, LParam lParam);
        LResult onWindowRefresh(WParam wParam, LParam lParam);
        LResult onWindowContentScaleChanged(WParam wParam, LParam lParam);

        LResult onKeyPressed(WParam wParam, LParam lParam);
        LResult onKeyReleased(WParam wParam, LParam lParam);
        LResult onChar(WParam wParam, LParam lParam);

        LResult onLeftMouseButtonPressed(WParam wParam, LParam lParam);
        LResult onRightMouseButtonPressed(WParam wParam, LParam lParam);
        LResult onMiddleMouseButtonPressed(WParam wParam, LParam lParam);
        LResult onExtraMouseButtonPressed(WParam wParam, LParam lParam);

        LResult onLeftMouseButtonReleased(WParam wParam, LParam lParam);
        LResult onRightMouseButtonReleased(WParam wParam, LParam lParam);
        LResult onMiddleMouseButtonReleased(WParam wParam, LParam lParam);
        LResult onExtraButtonReleased(WParam wParam, LParam lParam);

        LResult onMouseMoved(WParam wParam, LParam lParam);
        LResult onMouseScrolled(WParam wParam, LParam lParam);
        LResult onMouseLeft(WParam wParam, LParam lParam);
    };
}
    