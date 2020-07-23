#pragma once
#include <functional>

namespace Vortex
{
	enum struct EventType
	{
		WindowClose, WindowMinimize, WindowUnminimize, WindowMaximize, WindowUnmaximize,
		WindowResize, WindowMove
	};

	class IEvent
	{
	public:
		friend class EventDispatcher;

		virtual EventType GetType() = 0;
		bool IsHandled() { return m_IsHandled; }

	protected:
		bool m_IsHandled;
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(IEvent& event)
			: m_Event(event)
		{}

		template<typename T>
		bool Dispatch(std::function<bool(const T&)> function)
		{
			if (m_Event.m_IsHandled) return false;

			if (T.GetStaticType() == m_Event.GetType())
			{
				m_Event.m_IsHandled = function(static_cast<T>(m_Event));
				return m_Event.m_IsHandled;
			}
			return false;
		}

		template<typename T>
		bool Dispatch(std::function<bool(void)> function)
		{
			if (T.GetStaticType() == m_Event.GetType())
			{
				m_Event.m_IsHandled = function();
				return m_Event.m_IsHandled;
			}
			return false;
		}

		template<typename T>
		bool Dispatch(std::function<void(void)> function)
		{
			if (T::GetStaticType() == m_Event.GetType())
			{
				m_Event.m_IsHandled = false;
				function();
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
