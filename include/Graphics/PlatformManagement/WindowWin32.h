#pragma once
#include <string>
#include <iostream>
#include "WindowEvents.h"
#include "IOEvents.h"
//#include "Mouse.h"
//#include "Keyboard.h"
#include "Utilities/EventSystems/MultiEventSystem.h"
#include "Utilities/EventSystems/SingleCallbackEventSystem.h"


namespace Platform::Win32
{
    //windows are thread local, declare them thread_local for clarity
    class Window
    {
    private:
        class ClassRegistrator;

#ifdef _MAC
        static LongResult PASCAL windowProc(WindowHandle windowHandle, UnsignedInt unsignedMessage, WideParameter wideParameter, LongParameter longParameter);
#elif (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
        static LongResult __stdcall windowProc(WindowHandle windowHandle, UnsignedInt unsignedMessage, WideParameter wideParameter, LongParameter longParameter);
#else
        static LongResult windowProc(WindowHandle windowHandle, UnsignedInt unsignedMessage, WideParameter wideParameter, LongParameter longParameter);
#endif

    public:
        using EventManager = Utils::MultiEventSystem<Utils::SingleCallbackEventSystem, WindowEventPolicy, IOEventPolicy>;

    private:
        WindowHandle m_windowHandle = nullptr;        
        bool m_mouseInWindow = false;
        Position m_cursorPos;
        bool m_shouldClose = false;

        WindowAttributes m_attr;

        thread_local static inline Position s_cursorPosGlobal = { 0,0 };
        thread_local static inline size_t s_windowCounter = 0;        
    protected:
        EventManager m_windowEvents;

    public:
        Window() noexcept : m_windowHandle(nullptr), m_mouseInWindow(false), 
        m_cursorPos({0, 0}), m_shouldClose(false), m_windowEvents() {};
        Window(Extent extent, std::string_view title, WindowAttributes attr = WindowAttributes::defaultAtr());
        ~Window();

        Window(Window&& other) noexcept : m_windowHandle(nullptr), m_mouseInWindow(false), 
        m_cursorPos({0, 0}), m_shouldClose(false), m_windowEvents() {
            m_windowHandle = std::exchange(other.m_windowHandle, nullptr);
            m_windowEvents = std::exchange(other.m_windowEvents, EventManager());
            m_mouseInWindow = std::exchange(other.m_mouseInWindow, false);
            if (m_windowHandle) {
                setThisPointer();
            }
        };

        Window& operator=(Window&& other) noexcept
        {
            if (this == &other)
                return *this;
            destroy();
            m_windowHandle = std::exchange(other.m_windowHandle, nullptr);
            m_windowEvents = std::exchange(other.m_windowEvents, EventManager());
            m_mouseInWindow = std::exchange(other.m_mouseInWindow, false);
            if (m_windowHandle) {
                setThisPointer();
            }
            return *this;
        };

        Window(const Window&) noexcept = delete;
        Window& operator=(const Window&) noexcept = delete;

        // returns a platform native handle
        WindowHandle getHandle() const { return m_windowHandle; }

        void pollEvents();
        static void pollEventsGlobal();
        bool popEvent();
        static bool popEventGlobal();
        void show();
        void hide();
        void minimise();
        void maximise();
        void restore();

        void destroy();
        void create(Extent extent, std::string_view title, WindowAttributes attr = WindowAttributes::defaultAtr());

        bool shouldClose() const;

        void setCursorPos(Position pos);
        void setResizable(bool resizable);
        void setVisible(bool visible);
        void setDecorated(bool decorated);
        void setFocused(bool focused);
        void setAutoIconify(bool autoIconify);
        void setFloating(bool floating);
        void setMaximized(bool maximized);
        void centerCursor(bool center);
        void setTransparentFramebuffer(bool transparent);
        void setFocusOnShow(bool focusOnShow);
        void setCursorMode(CursorMode mode);

        bool isResizable() const;
        bool isVisible() const;
        bool isDecorated() const;
        bool isFocused() const;
        bool isAutoIconify() const { return true; } //always true on windows
        bool isFloating() const;
        bool isMinimised();
        bool isMaximised();
        bool isCursorCentered() const { return m_attr.centerCursor; }
        bool isTransparentFramebuffer() const;
        bool isFocusOnShow() const { return m_attr.focusOnShow; }
        CursorMode getCursorMode() const { return m_attr.cursorMode; }
        Position getCursorPos();
        static Position getGlobalCursorPos();
        void centerCursor();

        Extent getWindowExtent() const;
        Extent getClientExtent() const;
        Extent getFrameBufferExtent() const;
        Position getWindowPosition() const;
        float getDPIScale() const;

        static InstanceHandle getInstanceHandle();

    private:
        void setThisPointer();
        static Window* getThisPointer(WindowHandle windowHandle);

        template <LongResult(Window::* func)(WideParameter, LongParameter)>
        static inline LongResult callEvent(WindowHandle windowHandle, UnsignedInt unsignedMessage, WideParameter wideParameter, LongParameter longParameter) {
            Window* window = getThisPointer(windowHandle);
            if (window)
            {
                window->m_windowEvents.emit<WindowEvents::Native>(windowHandle, unsignedMessage, wideParameter, longParameter);
                return (window->*func)(wideParameter, longParameter);
            }
            return 0;
        }

        static LongResult onNull(WideParameter WideParameter, LongParameter LongParameter);
        static LongResult onCreate(WindowHandle WindowHandle, WideParameter WideParameter, LongParameter LongParameter);
        LongResult onDestroy(WideParameter WideParameter, LongParameter LongParameter);

        LongResult onWindowResized(WideParameter WideParameter, LongParameter LongParameter);
        LongResult onWindowMoved(WideParameter WideParameter, LongParameter LongParameter);
        LongResult onWindowFocused(WideParameter WideParameter, LongParameter LongParameter);
        LongResult onWindowUnfocused(WideParameter WideParameter, LongParameter LongParameter);
        LongResult onWindowClosed(WideParameter WideParameter, LongParameter LongParameter);
        LongResult onWindowRefresh(WideParameter WideParameter, LongParameter LongParameter);
        LongResult onWindowContentScaleChanged(WideParameter WideParameter, LongParameter LongParameter);

        LongResult onKeyPressed(WideParameter WideParameter, LongParameter LongParameter);
        LongResult onKeyReleased(WideParameter WideParameter, LongParameter LongParameter);
        LongResult onChar(WideParameter WideParameter, LongParameter LongParameter);

        LongResult onLeftMouseButtonPressed(WideParameter WideParameter, LongParameter LongParameter);
        LongResult onRightMouseButtonPressed(WideParameter WideParameter, LongParameter LongParameter);
        LongResult onMiddleMouseButtonPressed(WideParameter WideParameter, LongParameter LongParameter);
        LongResult onExtraMouseButtonPressed(WideParameter WideParameter, LongParameter LongParameter);

        LongResult onLeftMouseButtonReleased(WideParameter WideParameter, LongParameter LongParameter);
        LongResult onRightMouseButtonReleased(WideParameter WideParameter, LongParameter LongParameter);
        LongResult onMiddleMouseButtonReleased(WideParameter WideParameter, LongParameter LongParameter);
        LongResult onExtraButtonReleased(WideParameter WideParameter, LongParameter LongParameter);

        LongResult onMouseMoved(WideParameter WideParameter, LongParameter LongParameter);
        LongResult onMouseScrolled(WideParameter WideParameter, LongParameter LongParameter);
        LongResult onMouseLeft(WideParameter WideParameter, LongParameter LongParameter);

    protected:
        void updateCursorState();        
    };
}
    