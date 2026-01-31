#pragma once
#ifdef _WIN32
#include "WindowWin32.h"
#else
#include "WindowXlib.h"
#endif

namespace Platform
{
#ifdef _WIN32
	using WindowImpl = Win32::Window;
#else
	using WindowImpl = Xlib::Window;
#endif

	class Window : public WindowImpl
	{
	public:
		using EventManager = WindowImpl::EventManager;

		using WindowImpl::WindowImpl;

		template<auto E>
		Window& registerCallback(auto callback)
		{
			m_windowEvents.set<E>(std::move(callback));
			return *this;
		}

		template<auto E>
		Window& registerCallback(auto callback, auto& handler)
		{
			m_windowEvents.set<E>(std::move(callback), handler);
			return *this;
		}

		template<auto E, typename... Args>
		Window& emitEvent(Args... args) {
			m_windowEvents.emit<E>(std::forward<Args>(args)...);
			return *this;
		};

	};
}