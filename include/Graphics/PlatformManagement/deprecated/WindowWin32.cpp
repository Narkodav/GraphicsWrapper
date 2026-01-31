#include "WindowWin32.h"
#include "WindowWin32ClassRegistrator.h"

namespace Platform
{
    WindowWin32ClassRegistrator::WindowWin32ClassRegistrator() {
        m_wc.lpfnWndProc = WindowProc;
        m_wc.hInstance = GetModuleHandle(nullptr);
        m_wc.lpszClassName = m_name.c_str();
        RegisterClass(&m_wc);
    }

    WindowWin32::WindowWin32(int width, int height, std::wstring_view title)
    {        
        auto& wc = WindowWin32ClassRegistrator::getInstance();
        m_hwnd = CreateWindowEx(0, wc.getName().c_str(), title.data(), WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, reinterpret_cast<HINSTANCE>(getHInstance()), this);
    }

    WindowWin32::~WindowWin32()
    {
        destroy();
    }

    void WindowWin32::destroy() {
        if (m_hwnd) {
            DestroyWindow(reinterpret_cast<HWND>(m_hwnd));
        }
    }

    void WindowWin32::create(int width, int height, std::wstring_view title) {
        destroy();
        auto& wc = WindowWin32ClassRegistrator::getInstance();
        m_hwnd = CreateWindowEx(0, wc.getName().c_str(), title.data(), WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, reinterpret_cast<HINSTANCE>(getHInstance()), this);
    }

    void WindowWin32::setThisPointer()
    {
        SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    }

    void WindowWin32::pollEvents() {
        MSG msg;
        while (PeekMessage(&msg, m_hwnd, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    void WindowWin32::pollEvents() {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    bool WindowWin32::popEvent() {
        MSG msg;
        bool hadEvent = PeekMessage(&msg, m_hwnd, 0, 0, PM_REMOVE);
        if (!hadEvent)
            return false;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        return true;
    }

    bool WindowWin32::popEvent() {
        MSG msg;
        bool hadEvent = PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);
        if (!hadEvent)
            return false;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        return true;
    }

    void WindowWin32::show() const { ShowWindow(m_hwnd, SW_SHOW); }

    HInstance WindowWin32::getHInstance() { return GetModuleHandle(nullptr); };

    WindowWin32* WindowWin32::getThisPointer(HWnd hwnd) {
        return reinterpret_cast<WindowWin32*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    LRESULT CALLBACK WindowWin32::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg) {
        case WM_NULL: return onNull(wParam, lParam);
        case WM_CREATE: return onCreate(wParam, lParam);
        case WM_DESTROY: return callEvent<&WindowWin32::onDestroy>(hwnd, wParam, lParam);
        
        // Window events
        case WM_SIZE: return callEvent<&WindowWin32::onWindowResized>(hwnd, wParam, lParam);
        case WM_MOVE: return callEvent<&WindowWin32::onWindowMoved>(hwnd, wParam, lParam);
        case WM_SETFOCUS: return callEvent<&WindowWin32::onWindowFocused>(hwnd, wParam, lParam);
        case WM_KILLFOCUS: return callEvent<&WindowWin32::onWindowUnfocused>(hwnd, wParam, lParam);
        case WM_CLOSE: return callEvent<&WindowWin32::onWindowClosed>(hwnd, wParam, lParam);
        case WM_PAINT: return callEvent<&WindowWin32::onWindowRefresh>(hwnd, wParam, lParam);
        case WM_DPICHANGED: return callEvent<&WindowWin32::onWindowContentScaleChanged>(hwnd, wParam, lParam);
        
        // Keyboard events
        case WM_KEYDOWN: return callEvent<&WindowWin32::onKeyPressed>(hwnd, wParam, lParam);
        case WM_KEYUP: return callEvent<&WindowWin32::onKeyReleased>(hwnd, wParam, lParam);
        case WM_CHAR: return callEvent<&WindowWin32::onChar>(hwnd, wParam, lParam);

        // Mouse events
        case WM_LBUTTONDOWN: return callEvent<&WindowWin32::onLeftMouseButtonPressed>(hwnd, wParam, lParam);
        case WM_RBUTTONDOWN: return callEvent<&WindowWin32::onRightMouseButtonPressed>(hwnd, wParam, lParam);
        case WM_MBUTTONDOWN: return callEvent<&WindowWin32::onMiddleMouseButtonPressed>(hwnd, wParam, lParam);
        case WM_XBUTTONDOWN: return callEvent<&WindowWin32::onExtraMouseButtonPressed>(hwnd, wParam, lParam);
        case WM_LBUTTONUP: return callEvent<&WindowWin32::onLeftMouseButtonReleased>(hwnd, wParam, lParam);
        case WM_RBUTTONUP: return callEvent<&WindowWin32::onRightMouseButtonReleased>(hwnd, wParam, lParam);
        case WM_MBUTTONUP: return callEvent<&WindowWin32::onMiddleMouseButtonReleased>(hwnd, wParam, lParam);
        case WM_XBUTTONUP: return callEvent<&WindowWin32::onExtraButtonReleased>(hwnd, wParam, lParam);
        case WM_MOUSEMOVE: return callEvent<&WindowWin32::onMouseMoved>(hwnd, wParam, lParam);
        case WM_MOUSEWHEEL:
        case WM_MOUSEHWHEEL: return callEvent<&WindowWin32::onMouseScrolled>(hwnd, wParam, lParam);
        case WM_MOUSELEAVE: return callEvent<&WindowWin32::onMouseLeft>(hwnd, wParam, lParam);
        
        default: 
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    LRESULT WindowWin32::onNull(WPARAM wParam, LPARAM lParam)
    {
        return 0;
    }

    LRESULT WindowWin32::onCreate(WPARAM wParam, LPARAM lParam)
    {
        ++s_windowCounter;
        CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        WindowWin32* window = static_cast<WindowWin32*>(cs->lpCreateParams);
        window->setThisPointer();
        return 0;
    }

    LRESULT WindowWin32::onDestroy(WPARAM wParam, LPARAM lParam)
    {
        m_hwnd = nullptr;
        --s_windowCounter;
        if(s_windowCounter == 0)
            PostQuitMessage(0);
        return 0;
    }

    LRESULT WindowWin32::onWindowResized(WPARAM wParam, LPARAM lParam)
    {
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);

        switch (wParam)
        {
        case SIZE_RESTORED:
            m_windowEvents.emit<WindowEvents::WindowResized>(width, height); return 0;
        case SIZE_MINIMIZED:
            m_windowEvents.emit<WindowEvents::WindowMinimized>(width, height); return 0;
        case SIZE_MAXIMIZED:
            m_windowEvents.emit<WindowEvents::WindowMaximized>(width, height); return 0;
        default: return 0;
        }
        return 0;
    }

    LRESULT WindowWin32::onWindowMoved(WPARAM wParam, LPARAM lParam)
    {
        int32_t x = static_cast<int16_t>(LOWORD(lParam));
        int32_t y = static_cast<int16_t>(HIWORD(lParam));
        m_windowEvents.emit<WindowEvents::WindowMoved>(x, y);
        return 0;
    }

    LRESULT WindowWin32::onWindowFocused(WPARAM wParam, LPARAM lParam)
    {
        m_windowEvents.emit<WindowEvents::WindowFocused>();
        return 0;
    }

    LRESULT WindowWin32::onWindowUnfocused(WPARAM wParam, LPARAM lParam)
    {
        m_windowEvents.emit<WindowEvents::WindowUnfocused>();
        return 0;
    }

    LRESULT WindowWin32::onWindowClosed(WPARAM wParam, LPARAM lParam)
    {
        m_windowEvents.emit<WindowEvents::WindowClosed>();
        return 0;
    }

    LRESULT WindowWin32::onWindowRefresh(WPARAM wParam, LPARAM lParam)
    {
        //RECT updateRect;
        //bool hasUpdateRegion = GetUpdateRect(m_hwnd, &updateRect, FALSE);
        m_windowEvents.emit<WindowEvents::WindowRefresh>();
        return 0;
    }

    LRESULT WindowWin32::onWindowContentScaleChanged(WPARAM wParam, LPARAM lParam)
    {
        float dpiX = HIWORD(wParam);
        float dpiY = LOWORD(wParam);
        //auto suggestedRect = reinterpret_cast<RECT*>(lParam);
        m_windowEvents.emit<WindowEvents::WindowContentScaleChanged>(dpiX, dpiY);
        return 0;
    }

    LRESULT WindowWin32::onKeyPressed(WPARAM wParam, LPARAM lParam)
    {
        auto key = static_cast<KeyboardKey>(wParam);
        bool isExtended = (lParam & (1 << 24));
        bool wasDown = (lParam & (1 << 30));
        uint32_t scanCode = (lParam >> 16) & 0xFF;

        if(wasDown)
            m_windowEvents.emit<IOEvents::KeyRepeated>(key, scanCode);
        else m_windowEvents.emit<IOEvents::KeyPressed>(key, scanCode);
        return 0;
    }

    LRESULT WindowWin32::onKeyReleased(WPARAM wParam, LPARAM lParam)
    {
        auto key = static_cast<KeyboardKey>(wParam);
        uint32_t scanCode = (lParam >> 16) & 0xFF;
        m_windowEvents.emit<IOEvents::KeyReleased>(key, scanCode);
        return 0;
    }

    LRESULT WindowWin32::onChar(WPARAM wParam, LPARAM lParam)
    {
        unsigned int codepoint = static_cast<unsigned int>(wParam);
        bool isExtendedKey = (lParam & (1 << 24));
        bool isAltPressed = (lParam & (1 << 29));
        bool wasDownBefore = (lParam & (1 << 30));
        m_windowEvents.emit<IOEvents::CharInput>(codepoint, isExtendedKey, isAltPressed, wasDownBefore);
        return 0;
    }

    LRESULT WindowWin32::onLeftMouseButtonPressed(WPARAM wParam, LPARAM lParam) {
        double x = LOWORD(lParam);
        double y = HIWORD(lParam);
        m_windowEvents.emit<IOEvents::MouseLeftButtonPressed>(lParam, lParam);
        m_windowEvents.emit<IOEvents::MouseButtonPressed>(MouseButton::Lmb, lParam, lParam);
        return 0;
    }

    LRESULT WindowWin32::onRightMouseButtonPressed(WPARAM wParam, LPARAM lParam) {
        double x = LOWORD(lParam);
        double y = HIWORD(lParam);
        m_windowEvents.emit<IOEvents::MouseRightButtonPressed>(lParam, lParam);
        m_windowEvents.emit<IOEvents::MouseButtonPressed>(MouseButton::Rmb, lParam, lParam);
        return 0;
    }
    LRESULT WindowWin32::onMiddleMouseButtonPressed(WPARAM wParam, LPARAM lParam) {
        double x = LOWORD(lParam);
        double y = HIWORD(lParam);
        m_windowEvents.emit<IOEvents::MouseMiddleButtonPressed>(lParam, lParam);
        m_windowEvents.emit<IOEvents::MouseButtonPressed>(MouseButton::Mmb, lParam, lParam);
        return 0;
    }
    LRESULT WindowWin32::onExtraMouseButtonPressed(WPARAM wParam, LPARAM lParam) {
        WORD xButton = HIWORD(wParam);
        double x = LOWORD(lParam);
        double y = HIWORD(lParam);
        if (xButton == XBUTTON1) {
            m_windowEvents.emit<IOEvents::MouseExtraButtonPressed>(MouseButton::Button4, lParam, lParam);
            m_windowEvents.emit<IOEvents::MouseButtonPressed>(MouseButton::Button4, lParam, lParam);
        }
        else if (xButton == XBUTTON2) {
            m_windowEvents.emit<IOEvents::MouseExtraButtonPressed>(MouseButton::Button5, lParam, lParam);
            m_windowEvents.emit<IOEvents::MouseButtonPressed>(MouseButton::Button5, lParam, lParam);
        }
        else m_windowEvents.emit<IOEvents::MouseButtonPressed>(MouseButton::Unknown, lParam, lParam);
        return 0;
    }

    LRESULT WindowWin32::onLeftMouseButtonReleased(WPARAM wParam, LPARAM lParam) {
        double x = LOWORD(lParam);
        double y = HIWORD(lParam);
        m_windowEvents.emit<IOEvents::MouseLeftButtonReleased>(lParam, lParam);
        m_windowEvents.emit<IOEvents::MouseButtonReleased>(MouseButton::Lmb, lParam, lParam);
        return 0;
    }
    LRESULT WindowWin32::onRightMouseButtonReleased(WPARAM wParam, LPARAM lParam) {
        double x = LOWORD(lParam);
        double y = HIWORD(lParam);
        m_windowEvents.emit<IOEvents::MouseRightButtonReleased>(lParam, lParam);
        m_windowEvents.emit<IOEvents::MouseButtonReleased>(MouseButton::Rmb, lParam, lParam);
        return 0;
    }
    LRESULT WindowWin32::onMiddleMouseButtonReleased(WPARAM wParam, LPARAM lParam) {
        double x = LOWORD(lParam);
        double y = HIWORD(lParam);
        m_windowEvents.emit<IOEvents::MouseMiddleButtonReleased>(lParam, lParam);
        m_windowEvents.emit<IOEvents::MouseButtonReleased>(MouseButton::Mmb, lParam, lParam);
        return 0;
    }
    LRESULT WindowWin32::onExtraButtonReleased(WPARAM wParam, LPARAM lParam) {
        WORD xButton = HIWORD(wParam);
        double x = LOWORD(lParam);
        double y = HIWORD(lParam);
        if (xButton == XBUTTON1) {
            m_windowEvents.emit<IOEvents::MouseExtraButtonReleased>(MouseButton::Button4, lParam, lParam);
            m_windowEvents.emit<IOEvents::MouseButtonReleased>(MouseButton::Button4, lParam, lParam);
        }
        else if (xButton == XBUTTON2) {
            m_windowEvents.emit<IOEvents::MouseExtraButtonReleased>(MouseButton::Button5, lParam, lParam);
            m_windowEvents.emit<IOEvents::MouseButtonReleased>(MouseButton::Button5, lParam, lParam);
        }
        else m_windowEvents.emit<IOEvents::MouseButtonReleased>(MouseButton::Unknown, lParam, lParam);
        return 0;
    }

    LRESULT WindowWin32::onMouseMoved(WPARAM wParam, LPARAM lParam) {
        double x = LOWORD(lParam);
        double y = HIWORD(lParam);
        m_windowEvents.emit<IOEvents::MouseMoved>(lParam, lParam);
        if(!m_mouseInWindow) 
        {
            m_windowEvents.emit<IOEvents::MouseEntered>(lParam, lParam);
            m_mouseInWindow = true;
        }
        return 0;
    }
    LRESULT WindowWin32::onMouseScrolled(WPARAM wParam, LPARAM lParam) {
        double xOffset = LOWORD(lParam);
        double yOffset = HIWORD(lParam);
        m_windowEvents.emit<IOEvents::MouseScrolled>(xOffset, yOffset);
    }
    LRESULT WindowWin32::onMouseLeft(WPARAM wParam, LPARAM lParam) {
        m_mouseInWindow = false;
        m_windowEvents.emit<IOEvents::MouseLeft>();
    }
}
