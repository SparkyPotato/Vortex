#pragma once
#include <functional>

namespace Vortex
{
	enum struct EventType
	{
		WindowClose, WindowMinimize, WindowUnminimize, WindowMaximize, WindowUnmaximize,
		WindowResize, WindowMove, WindowActivate, WindowDeactivate,
		KeyDown, KeyUp, MouseMove, MouseButtonDown, MouseButtonUp, MouseButtonDoubleClick, MouseScroll
	};

	class IEvent
	{
	public:
		friend class EventDispatcher;

		virtual EventType GetType() = 0;
		bool IsHandled() { return m_IsHandled; }

	protected:
		bool m_IsHandled = false;
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(IEvent& event)
			: m_Event(event)
		{}

		template<typename T>
		bool Dispatch(const std::function<bool(T&)>& function)
		{
			if (m_Event.m_IsHandled) return false;

			if (T::GetStaticType() == m_Event.GetType())
			{
				m_Event.m_IsHandled = function(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}

	private:
		IEvent& m_Event;
	};

	#define DEFINETYPES(eventType) virtual EventType GetType() override { return eventType; } \
								   static EventType GetStaticType() { return eventType; } 
}
