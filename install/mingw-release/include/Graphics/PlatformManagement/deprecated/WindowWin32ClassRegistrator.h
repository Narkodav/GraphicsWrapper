#pragma once
#include <windows.h>
#include <string>

namespace Platform
{
    class WindowWin32ClassRegistrator {
    private:
        WNDCLASS m_wc = {};
        const std::wstring m_name = L"WindowWin32";

        WindowWin32ClassRegistrator();
        ~WindowWin32ClassRegistrator() = default;
        WindowWin32ClassRegistrator(const WindowWin32ClassRegistrator&) = delete;
        WindowWin32ClassRegistrator& operator=(const WindowWin32ClassRegistrator&) = delete;

    public:

        WindowWin32ClassRegistrator(WindowWin32ClassRegistrator&&) = default;
        WindowWin32ClassRegistrator& operator=(WindowWin32ClassRegistrator&&) = default;

        const std::wstring& getName() const { return m_name; };
        WNDCLASS getHandle() const { return m_wc; };

        static WindowWin32ClassRegistrator& getInstance() {
            static WindowWin32ClassRegistrator instance;
            return instance;
        }
    };
}

