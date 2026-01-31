#include "Graphics/PlatformManagement/Window.h"

#ifdef STRICT
#undef STRICT
#endif
#define STRICT 1

// Also control related defines
#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

namespace Platform::Win32
{
    //typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);

    class Window::ClassRegistrator {
    private:
        WNDCLASS m_wc = {};
        const std::string m_name = "Window";

        ClassRegistrator() {
            m_wc.lpfnWndProc = reinterpret_cast<WNDPROC>(windowProc);
            m_wc.hInstance = GetModuleHandle(nullptr);
            m_wc.lpszClassName = m_name.c_str();
            RegisterClass(&m_wc);
        }
        ~ClassRegistrator() {
            UnregisterClass(m_wc.lpszClassName, m_wc.hInstance);
        }
        ClassRegistrator(const ClassRegistrator&) = delete;
        ClassRegistrator& operator=(const ClassRegistrator&) = delete;

    public:

        ClassRegistrator(ClassRegistrator&&) = default;
        ClassRegistrator& operator=(ClassRegistrator&&) = default;

        const std::string& getName() const { return m_name; };
        WNDCLASS getHandle() const { return m_wc; };

        static ClassRegistrator& getInstance() {
            static ClassRegistrator instance;
            return instance;
        }
    };

    Window::Window(Extent extent, std::string_view title, WindowAttributes attr /*= WindowAttributes::defaultAtr()*/)
    {
        create(std::move(extent), std::move(title), std::move(attr));
    }

    Window::~Window()
    {
        destroy();
    }

    void Window::destroy() {
        if (m_windowHandle) {
            m_shouldClose = true;
            DestroyWindow(reinterpret_cast<HWND>(m_windowHandle));
        }
    }

    void Window::create(Extent extent, std::string_view title, WindowAttributes attr /*= WindowAttributes::defaultAtr()*/) {
        destroy();
        m_attr = attr;

        auto& wc = ClassRegistrator::getInstance();

        // Convert WindowAttributes to Win32 window styles
        DWORD style = WS_OVERLAPPEDWINDOW;
        DWORD exStyle = 0;

        // Apply resizable attribute
        if (!m_attr.resizable) {
            style &= ~WS_THICKFRAME;
            style &= ~WS_MAXIMIZEBOX;
        }

        // Apply decorated attribute
        if (!m_attr.decorated) {
            style &= ~WS_CAPTION;
            style &= ~WS_SYSMENU;
            style &= ~WS_MINIMIZEBOX;
            style &= ~WS_MAXIMIZEBOX;

            // For borderless window, we might need to adjust
            if (m_attr.decorated) {
                style |= WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
                if (m_attr.resizable) {
                    style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
                }
            }
        }

        // Apply floating attribute (always on top)
        if (m_attr.floating) {
            exStyle |= WS_EX_TOPMOST;
        }

        // Apply transparent framebuffer
        if (m_attr.transparentFramebuffer) {
            exStyle |= WS_EX_LAYERED;
        }

        // Create the window
        m_windowHandle = reinterpret_cast<WindowHandle>(CreateWindowEx(
            exStyle,
            wc.getName().c_str(),
            title.data(),
            style,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            extent.width,
            extent.height,
            nullptr,
            nullptr,
            reinterpret_cast<HINSTANCE>(getInstanceHandle()),
            this
        ));

        if (!m_windowHandle) {
            throw std::runtime_error("Failed to create window");
        }

        // Update floating (always on top) state
        SetWindowPos(reinterpret_cast<HWND>(m_windowHandle),
            m_attr.floating ? HWND_TOPMOST : HWND_NOTOPMOST,
            0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

        // Update transparency
        if (m_attr.transparentFramebuffer) {
            SetLayeredWindowAttributes(reinterpret_cast<HWND>(m_windowHandle), 0, 255, LWA_ALPHA);
        }

        // Show window with appropriate command based on maximized state
        int showCommand = attr.maximized ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL;
        if (!attr.visible) {
            showCommand = SW_HIDE;
        }
        ShowWindow(reinterpret_cast<HWND>(m_windowHandle), showCommand);

        // Apply focus if requested
        if (attr.focused) {
            SetFocus(reinterpret_cast<HWND>(m_windowHandle));
        }

        setCursorMode(attr.cursorMode);

        if (m_attr.centerCursor || m_attr.cursorMode == CursorMode::Disabled) {
            centerCursor();
        }

        UpdateWindow(reinterpret_cast<HWND>(m_windowHandle));
        m_shouldClose = false;
    }

    void Window::pollEvents() {
        MSG msg;
        while (PeekMessage(&msg, reinterpret_cast<HWND>(m_windowHandle), 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    void Window::pollEventsGlobal() {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    bool Window::popEvent() {
        MSG msg;
        bool hadEvent = PeekMessage(&msg, reinterpret_cast<HWND>(m_windowHandle), 0, 0, PM_REMOVE);
        if (!hadEvent)
            return false;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        return true;
    }

    bool Window::popEventGlobal() {
        MSG msg;
        bool hadEvent = PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);
        if (!hadEvent)
            return false;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        return true;
    }

    void Window::show() { ShowWindow(reinterpret_cast<HWND>(m_windowHandle), SW_SHOW); }
    void Window::hide() { ShowWindow(reinterpret_cast<HWND>(m_windowHandle), SW_HIDE); }
    void Window::minimise() {
        if (!m_windowHandle) return;
        ShowWindow(reinterpret_cast<HWND>(m_windowHandle), SW_MINIMIZE);
    }

    void Window::maximise() {
        if (!m_windowHandle) return;
        ShowWindow(reinterpret_cast<HWND>(m_windowHandle), SW_MINIMIZE);
    }

    void Window::restore() {
        if (!m_windowHandle) return;
        ShowWindow(reinterpret_cast<HWND>(m_windowHandle), SW_RESTORE);
    }

    InstanceHandle Window::getInstanceHandle() { return reinterpret_cast<InstanceHandle>(GetModuleHandle(nullptr)); };

    void Window::setThisPointer()
    {
        SetWindowLongPtr(reinterpret_cast<HWND>(m_windowHandle), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    }

    Window* Window::getThisPointer(WindowHandle hwnd) {
        return reinterpret_cast<Window*>(GetWindowLongPtr(reinterpret_cast<HWND>(hwnd), GWLP_USERDATA));
    }

    LongResult CALLBACK Window::windowProc(WindowHandle hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg) {
        case WM_NULL: return onNull(wParam, lParam);
        case WM_CREATE: return onCreate(hwnd, wParam, lParam);
        case WM_DESTROY: return callEvent<&Window::onDestroy>(hwnd, uMsg, wParam, lParam);
        
        // Window events
        case WM_SIZE: return callEvent<&Window::onWindowResized>(hwnd, uMsg, wParam, lParam);
        case WM_MOVE: return callEvent<&Window::onWindowMoved>(hwnd, uMsg, wParam, lParam);
        case WM_SETFOCUS: return callEvent<&Window::onWindowFocused>(hwnd, uMsg, wParam, lParam);
        case WM_KILLFOCUS: return callEvent<&Window::onWindowUnfocused>(hwnd, uMsg, wParam, lParam);
        case WM_CLOSE: return callEvent<&Window::onWindowClosed>(hwnd, uMsg, wParam, lParam);
        case WM_PAINT: return callEvent<&Window::onWindowRefresh>(hwnd, uMsg, wParam, lParam);
        case WM_DPICHANGED: return callEvent<&Window::onWindowContentScaleChanged>(hwnd, uMsg, wParam, lParam);
        
        // Keyboard events
        case WM_KEYDOWN: return callEvent<&Window::onKeyPressed>(hwnd, uMsg, wParam, lParam);
        case WM_KEYUP: return callEvent<&Window::onKeyReleased>(hwnd, uMsg, wParam, lParam);
        case WM_CHAR: return callEvent<&Window::onChar>(hwnd, uMsg, wParam, lParam);

        // Mouse events
        case WM_LBUTTONDOWN: return callEvent<&Window::onLeftMouseButtonPressed>(hwnd, uMsg, wParam, lParam);
        case WM_RBUTTONDOWN: return callEvent<&Window::onRightMouseButtonPressed>(hwnd, uMsg, wParam, lParam);
        case WM_MBUTTONDOWN: return callEvent<&Window::onMiddleMouseButtonPressed>(hwnd, uMsg, wParam, lParam);
        case WM_XBUTTONDOWN: return callEvent<&Window::onExtraMouseButtonPressed>(hwnd, uMsg, wParam, lParam);
        case WM_LBUTTONUP: return callEvent<&Window::onLeftMouseButtonReleased>(hwnd, uMsg, wParam, lParam);
        case WM_RBUTTONUP: return callEvent<&Window::onRightMouseButtonReleased>(hwnd, uMsg, wParam, lParam);
        case WM_MBUTTONUP: return callEvent<&Window::onMiddleMouseButtonReleased>(hwnd, uMsg, wParam, lParam);
        case WM_XBUTTONUP: return callEvent<&Window::onExtraButtonReleased>(hwnd, uMsg, wParam, lParam);
        case WM_MOUSEMOVE: return callEvent<&Window::onMouseMoved>(hwnd, uMsg, wParam, lParam);
        case WM_MOUSEWHEEL:
        case WM_MOUSEHWHEEL: return callEvent<&Window::onMouseScrolled>(hwnd, uMsg, wParam, lParam);
        case WM_MOUSELEAVE: return callEvent<&Window::onMouseLeft>(hwnd, uMsg, wParam, lParam);
        
        default: 
            return DefWindowProc(reinterpret_cast<HWND>(hwnd), uMsg, wParam, lParam);
        }
    }

    LRESULT Window::onNull(WPARAM, LPARAM)
    {
        return 0;
    }

    LRESULT Window::onCreate(WindowHandle hwnd, WPARAM, LPARAM lParam)
    {
        ++s_windowCounter;
        CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        Window* window = static_cast<Window*>(cs->lpCreateParams);
        window->m_windowHandle = reinterpret_cast<WindowHandle>(hwnd);
        window->setThisPointer();
        return 0;
    }

    LRESULT Window::onDestroy(WPARAM, LPARAM)
    {
        m_windowHandle = nullptr;
        --s_windowCounter;
        if(s_windowCounter == 0)
            PostQuitMessage(0);
        return 0;
    }

    LRESULT Window::onWindowResized(WPARAM wParam, LPARAM lParam)
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

    LRESULT Window::onWindowMoved(WPARAM, LPARAM lParam)
    {
        int32_t x = static_cast<int16_t>(LOWORD(lParam));
        int32_t y = static_cast<int16_t>(HIWORD(lParam));
        m_windowEvents.emit<WindowEvents::WindowMoved>(x, y);
        return 0;
    }

    LRESULT Window::onWindowFocused(WPARAM, LPARAM)
    {
        m_windowEvents.emit<WindowEvents::WindowFocused>();
        return 0;
    }

    LRESULT Window::onWindowUnfocused(WPARAM, LPARAM)
    {
        m_windowEvents.emit<WindowEvents::WindowUnfocused>();
        return 0;
    }

    LRESULT Window::onWindowClosed(WPARAM, LPARAM)
    {
        m_shouldClose = true;
        m_windowEvents.emit<WindowEvents::WindowClosed>();
        return 0;
    }

    LRESULT Window::onWindowRefresh(WPARAM, LPARAM)
    {
        HDC hdc = GetDC(reinterpret_cast<HWND>(m_windowHandle));
        ReleaseDC(reinterpret_cast<HWND>(m_windowHandle), hdc);
        ValidateRect(reinterpret_cast<HWND>(m_windowHandle), nullptr);
        m_windowEvents.emit<WindowEvents::WindowRefresh>();
        return 0;
    }

    LRESULT Window::onWindowContentScaleChanged(WPARAM wParam, LPARAM)
    {
        float dpiX = HIWORD(wParam);
        float dpiY = LOWORD(wParam);
        //auto suggestedRect = reinterpret_cast<RECT*>(lParam);
        m_windowEvents.emit<WindowEvents::WindowContentScaleChanged>(dpiX, dpiY);
        return 0;
    }

    LRESULT Window::onKeyPressed(WPARAM wParam, LPARAM lParam)
    {
        auto key = static_cast<KeyboardKey>(wParam);
        //bool isExtended = (lParam & (1 << 24));
        bool wasDown = (lParam & (1 << 30));
        uint32_t scanCode = (lParam >> 16) & 0xFF;

        if(wasDown)
            m_windowEvents.emit<IOEvents::KeyRepeated>(key, scanCode);
        else m_windowEvents.emit<IOEvents::KeyPressed>(key, scanCode);
        return 0;
    }

    LRESULT Window::onKeyReleased(WPARAM wParam, LPARAM lParam)
    {
        auto key = static_cast<KeyboardKey>(wParam);
        uint32_t scanCode = (lParam >> 16) & 0xFF;
        m_windowEvents.emit<IOEvents::KeyReleased>(key, scanCode);
        return 0;
    }

    LRESULT Window::onChar(WPARAM wParam, LPARAM lParam)
    {
        unsigned int codepoint = static_cast<unsigned int>(wParam);
        bool isExtendedKey = (lParam & (1 << 24));
        bool isAltPressed = (lParam & (1 << 29));
        bool wasDownBefore = (lParam & (1 << 30));
        m_windowEvents.emit<IOEvents::CharInput>(codepoint, isExtendedKey, isAltPressed, wasDownBefore);
        return 0;
    }

    LRESULT Window::onLeftMouseButtonPressed(WPARAM, LPARAM lParam) {
        double x = LOWORD(lParam);
        double y = HIWORD(lParam);
        m_windowEvents.emit<IOEvents::MouseLeftButtonPressed>(x, y);
        m_windowEvents.emit<IOEvents::MouseButtonPressed>(MouseButton::Lmb, x, y);
        return 0;
    }

    LRESULT Window::onRightMouseButtonPressed(WPARAM, LPARAM lParam) {
        double x = LOWORD(lParam);
        double y = HIWORD(lParam);
        m_windowEvents.emit<IOEvents::MouseRightButtonPressed>(x, y);
        m_windowEvents.emit<IOEvents::MouseButtonPressed>(MouseButton::Rmb, x, y);
        return 0;
    }
    LRESULT Window::onMiddleMouseButtonPressed(WPARAM, LPARAM lParam) {
        double x = LOWORD(lParam);
        double y = HIWORD(lParam);
        m_windowEvents.emit<IOEvents::MouseMiddleButtonPressed>(x, y);
        m_windowEvents.emit<IOEvents::MouseButtonPressed>(MouseButton::Mmb, x, y);
        return 0;
    }
    LRESULT Window::onExtraMouseButtonPressed(WPARAM wParam, LPARAM lParam) {
        WORD xButton = HIWORD(wParam);
        double x = LOWORD(lParam);
        double y = HIWORD(lParam);
        if (xButton == XBUTTON1) {
            m_windowEvents.emit<IOEvents::MouseExtraButtonPressed>(MouseButton::Button4, x, y);
            m_windowEvents.emit<IOEvents::MouseButtonPressed>(MouseButton::Button4, x, y);
        }
        else if (xButton == XBUTTON2) {
            m_windowEvents.emit<IOEvents::MouseExtraButtonPressed>(MouseButton::Button5, x, y);
            m_windowEvents.emit<IOEvents::MouseButtonPressed>(MouseButton::Button5, x, y);
        }
        else m_windowEvents.emit<IOEvents::MouseButtonPressed>(MouseButton::Unknown, x, y);
        return 0;
    }

    LRESULT Window::onLeftMouseButtonReleased(WPARAM, LPARAM lParam) {
        double x = LOWORD(lParam);
        double y = HIWORD(lParam);
        m_windowEvents.emit<IOEvents::MouseLeftButtonReleased>(x, y);
        m_windowEvents.emit<IOEvents::MouseButtonReleased>(MouseButton::Lmb, x, y);
        return 0;
    }
    LRESULT Window::onRightMouseButtonReleased(WPARAM, LPARAM lParam) {
        double x = LOWORD(lParam);
        double y = HIWORD(lParam);
        m_windowEvents.emit<IOEvents::MouseRightButtonReleased>(x, y);
        m_windowEvents.emit<IOEvents::MouseButtonReleased>(MouseButton::Rmb, x, y);
        return 0;
    }
    LRESULT Window::onMiddleMouseButtonReleased(WPARAM, LPARAM lParam) {
        double x = LOWORD(lParam);
        double y = HIWORD(lParam);
        m_windowEvents.emit<IOEvents::MouseMiddleButtonReleased>(x, y);
        m_windowEvents.emit<IOEvents::MouseButtonReleased>(MouseButton::Mmb, x, y);
        return 0;
    }
    LRESULT Window::onExtraButtonReleased(WPARAM wParam, LPARAM lParam) {
        WORD xButton = HIWORD(wParam);
        double x = LOWORD(lParam);
        double y = HIWORD(lParam);
        if (xButton == XBUTTON1) {
            m_windowEvents.emit<IOEvents::MouseExtraButtonReleased>(MouseButton::Button4, x, y);
            m_windowEvents.emit<IOEvents::MouseButtonReleased>(MouseButton::Button4, x, y);
        }
        else if (xButton == XBUTTON2) {
            m_windowEvents.emit<IOEvents::MouseExtraButtonReleased>(MouseButton::Button5, x, y);
            m_windowEvents.emit<IOEvents::MouseButtonReleased>(MouseButton::Button5, x, y);
        }
        else m_windowEvents.emit<IOEvents::MouseButtonReleased>(MouseButton::Unknown, x, y);
        return 0;
    }

    LRESULT Window::onMouseMoved(WPARAM, LPARAM lParam) {
        double x = LOWORD(lParam);
        double y = HIWORD(lParam);
        m_windowEvents.emit<IOEvents::MouseMoved>(x, y);
        if(!m_mouseInWindow) 
        {
            m_windowEvents.emit<IOEvents::MouseEntered>(x, y);
            m_mouseInWindow = true;
        }
        if (m_attr.centerCursor || m_attr.cursorMode == CursorMode::Disabled) {
            centerCursor();
        }
        return 0;
    }
    LRESULT Window::onMouseScrolled(WPARAM, LPARAM lParam) {
        double xOffset = LOWORD(lParam);
        double yOffset = HIWORD(lParam);
        m_windowEvents.emit<IOEvents::MouseScrolled>(xOffset, yOffset);
        return 0;
    }
    LRESULT Window::onMouseLeft(WPARAM, LPARAM) {
        m_mouseInWindow = false;
        m_windowEvents.emit<IOEvents::MouseLeft>();
        return 0;
    }

    bool Window::shouldClose() const
    {
        return m_shouldClose;
    }

    Position Window::getCursorPos()
    {
        POINT cursorPos;
        if (GetCursorPos(&cursorPos)) {
            ScreenToClient(reinterpret_cast<HWND>(m_windowHandle), &cursorPos);
            m_cursorPos.x = cursorPos.x;
            m_cursorPos.y = cursorPos.y;
        }
        return m_cursorPos;
    }

    Position Window::getGlobalCursorPos()
    {
        POINT cursorPos;
        if (GetCursorPos(&cursorPos)) {
            s_cursorPosGlobal.x = cursorPos.x;
            s_cursorPosGlobal.y = cursorPos.y;            
        }
        return s_cursorPosGlobal;
    }

    void Window::setCursorPos(Position pos)
    {
        SetCursorPos(pos.x, pos.y);
    }

    bool Window::isMinimised()
    {
        if (!m_windowHandle) return false;
        return IsIconic(reinterpret_cast<HWND>(m_windowHandle));
    }

    bool Window::isMaximised()
    {
        if (!m_windowHandle) return false;
        return IsZoomed(reinterpret_cast<HWND>(m_windowHandle)) != FALSE;
    }

    void Window::setResizable(bool resizable)
    {
        if (!m_windowHandle) return;
        LONG style = GetWindowLong(reinterpret_cast<HWND>(m_windowHandle), GWL_STYLE);
        if (resizable) {
            style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
        }
        else {
            style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
        }
        SetWindowLong(reinterpret_cast<HWND>(m_windowHandle), GWL_STYLE, style);
        // Force redraw
        SetWindowPos(reinterpret_cast<HWND>(m_windowHandle), nullptr, 0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
    }

    void Window::setVisible(bool visible)
    {
        if (!m_windowHandle) return;
        ShowWindow(reinterpret_cast<HWND>(m_windowHandle), visible ? SW_SHOW : SW_HIDE);
    }

    void Window::setDecorated(bool decorated)
    {
        if (!m_windowHandle) return;

        LONG style = GetWindowLong(reinterpret_cast<HWND>(m_windowHandle), GWL_STYLE);
        if (decorated) {
            style |= WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
        }
        else {
            style &= ~(WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_THICKFRAME | WS_MAXIMIZEBOX);
        }
        SetWindowLong(reinterpret_cast<HWND>(m_windowHandle), GWL_STYLE, style);

        SetWindowPos(reinterpret_cast<HWND>(m_windowHandle), nullptr, 0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
    }

    void Window::setFocused(bool focused)
    {
        if (!m_windowHandle) return;

        if (focused) {
            SetFocus(reinterpret_cast<HWND>(m_windowHandle));
            SetForegroundWindow(reinterpret_cast<HWND>(m_windowHandle));
        }
        // Note: There's no direct way to "unfocus" a window in Win32
        // The window loses focus when another window is activated
    }

    void Window::setAutoIconify(bool autoIconify)
    {
        (void)autoIconify;
        // Win32 automatically handles iconification on focus loss by default
        // This is mainly for API compatibility with GLFW
    }

    void Window::setFloating(bool floating)
    {
        if (!m_windowHandle) return;

        SetWindowPos(reinterpret_cast<HWND>(m_windowHandle),
            floating ? HWND_TOPMOST : HWND_NOTOPMOST,
            0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }

    void Window::setMaximized(bool maximized)
    {
        if (!m_windowHandle) return;
        ShowWindow(reinterpret_cast<HWND>(m_windowHandle), maximized ? SW_MAXIMIZE : SW_RESTORE);
    }

    void Window::centerCursor(bool center)
    {
        m_attr.centerCursor = center;
        if (m_attr.centerCursor) {
            centerCursor();
        }
    }

    void Window::setTransparentFramebuffer(bool transparent)
    {
        if (!m_windowHandle) return;

        if (transparent) {
            // Enable layered window for transparency
            SetWindowLong(reinterpret_cast<HWND>(m_windowHandle), GWL_EXSTYLE,
                GetWindowLong(reinterpret_cast<HWND>(m_windowHandle), GWL_EXSTYLE) | WS_EX_LAYERED);
            // Set to fully opaque by default
            SetLayeredWindowAttributes(reinterpret_cast<HWND>(m_windowHandle), 0, 255, LWA_ALPHA);
        }
        else {
            // Disable layered window
            SetWindowLong(reinterpret_cast<HWND>(m_windowHandle), GWL_EXSTYLE,
                GetWindowLong(reinterpret_cast<HWND>(m_windowHandle), GWL_EXSTYLE) & ~WS_EX_LAYERED);
        }
    }

    void Window::setFocusOnShow(bool focusOnShow)
    {
        m_attr.focusOnShow = focusOnShow;
        // This controls behavior when showing the window
        // We'll handle this in the showWindow/createWindow methods
    }

    void Window::setCursorMode(CursorMode mode)
    {
        m_attr.cursorMode = mode;
        updateCursorState();
    }

    void Window::updateCursorState()
    {
        if (!m_windowHandle) return;

        switch (m_attr.cursorMode) {
        case CursorMode::Normal:
            ShowCursor(TRUE);
            // Release cursor confinement
            ClipCursor(nullptr);
            break;

        case CursorMode::Hidden:
            ShowCursor(FALSE);
            ClipCursor(nullptr);
            break;

        case CursorMode::Disabled:
            ShowCursor(FALSE);
            // Confine cursor to window for raw motion input
            RECT rect;
            GetClientRect(reinterpret_cast<HWND>(m_windowHandle), &rect);
            MapWindowPoints(reinterpret_cast<HWND>(m_windowHandle), nullptr, (POINT*)(&rect), 2);
            ClipCursor(&rect);
            break;
        }
    }

    void Window::centerCursor()
    {
        if (!m_windowHandle) return;

        RECT rect;
        GetClientRect(reinterpret_cast<HWND>(m_windowHandle), &rect);

        POINT center;
        center.x = rect.left + (rect.right - rect.left) / 2;
        center.y = rect.top + (rect.bottom - rect.top) / 2;

        ClientToScreen(reinterpret_cast<HWND>(m_windowHandle), &center);
        SetCursorPos(center.x, center.y);
    }

    bool Window::isResizable() const
    {
        if (!m_windowHandle) return false;
        return (GetWindowLong(reinterpret_cast<HWND>(m_windowHandle), GWL_STYLE) & WS_THICKFRAME) != 0;
    }

    bool Window::isVisible() const
    {
        if (!m_windowHandle) return false;
        return IsWindowVisible(reinterpret_cast<HWND>(m_windowHandle)) != FALSE;
    }

    bool Window::isDecorated() const
    {
        if (!m_windowHandle) return false;
        return (GetWindowLong(reinterpret_cast<HWND>(m_windowHandle), GWL_STYLE) & WS_CAPTION) != 0;
    }

    bool Window::isFocused() const
    {
        if (!m_windowHandle) return false;
        return GetForegroundWindow() == reinterpret_cast<HWND>(m_windowHandle);
    }

    bool Window::isFloating() const
    {
        if (!m_windowHandle) return false;
        return (GetWindowLong(reinterpret_cast<HWND>(m_windowHandle), GWL_EXSTYLE) & WS_EX_TOPMOST) != 0;
    }

    bool Window::isTransparentFramebuffer() const
    {
        if (!m_windowHandle) return false;
        return (GetWindowLong(reinterpret_cast<HWND>(m_windowHandle), GWL_EXSTYLE) & WS_EX_LAYERED) != 0;
    }

    Extent Window::getWindowExtent() const
    {
        if (!m_windowHandle) return { 0, 0 };

        RECT rect;
        if (GetWindowRect(reinterpret_cast<HWND>(m_windowHandle), &rect)) {
            return {
                rect.right - rect.left,  // Total window width
                rect.bottom - rect.top   // Total window height
            };
        }
        return { 0, 0 };
    }

    Extent Window::getClientExtent() const
    {
        if (!m_windowHandle) return { 0, 0 };

        RECT rect;
        if (GetClientRect(reinterpret_cast<HWND>(m_windowHandle), &rect)) {
            return {
                rect.right - rect.left,  // Client area width
                rect.bottom - rect.top   // Client area height
            };
        }
        return { 0, 0 };
    }

    Extent Window::getFrameBufferExtent() const
    {
        if (!m_windowHandle) return { 0, 0 };

        // For most cases, framebuffer size = client area size
        // But with DPI scaling, they might differ
        RECT rect;
        if (GetClientRect(reinterpret_cast<HWND>(m_windowHandle), &rect)) {
            // Convert to pixels accounting for DPI scaling
            float dpiScale = getDPIScale();
            return {
                static_cast<int32_t>((rect.right - rect.left) * dpiScale),
                static_cast<int32_t>((rect.bottom - rect.top) * dpiScale)
            };
        }
        return { 0, 0 };
    }

    Position Window::getWindowPosition() const
    {
        if (!m_windowHandle) return { 0, 0 };

        RECT rect;
        if (GetWindowRect(reinterpret_cast<HWND>(m_windowHandle), &rect)) {
            return { rect.left, rect.top };
        }
        return { 0, 0 };
    }

    float Window::getDPIScale() const
    {
        if (!m_windowHandle) return 1.0f;

        // Modern DPI awareness (Windows 10+)
        HMODULE user32 = LoadLibraryA("user32.dll");
        if (user32) {
            typedef UINT(WINAPI* GetDpiForWindowFn)(HWND);
            auto getDpiForWindow = reinterpret_cast<GetDpiForWindowFn>(reinterpret_cast<void*>(
                GetProcAddress(user32, "GetDpiForWindow")));

            if (getDpiForWindow) {
                UINT dpi = getDpiForWindow(reinterpret_cast<HWND>(m_windowHandle));
                FreeLibrary(user32);
                return dpi / 96.0f;  // 96 DPI = 100% scale
            }
            FreeLibrary(user32);
        }

        // Fallback for older Windows versions
        HDC hdc = GetDC(reinterpret_cast<HWND>(m_windowHandle));
        if (hdc) {
            int dpiX = GetDeviceCaps(hdc, LOGPIXELSX);
            ReleaseDC(reinterpret_cast<HWND>(m_windowHandle), hdc);
            return dpiX / 96.0f;
        }

        return 1.0f;
    }

}
