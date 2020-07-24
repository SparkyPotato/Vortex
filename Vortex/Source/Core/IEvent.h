#pragma once
#include <functional>

namespace Vortex
{
	/*
		All possible types of event. There is a class for each - just add Event to the end.
		For example, KeyDown becomes KeyDownEvent.
	*/
	enum struct EventType
	{
		WindowClose, WindowMinimize, WindowUnminimize, WindowMaximize, WindowUnmaximize,
		WindowResize, WindowMove, WindowActivate, WindowDeactivate,
		KeyDown, KeyUp, MouseMove, MouseButtonDown, MouseButtonUp, MouseButtonDoubleClick, MouseScroll
	};

	/*
		Base class of all events. 
	*/
	class IEvent
	{
	public:
		friend class EventDispatcher;

		/*
			Gets the type of the event from the derived class.
		*/
		virtual EventType GetType() = 0;
		/*
			Gets if the event has been handled already or not.
			The only way to handle and event is by using an EventDispatcher.
		*/
		bool IsHandled() { return m_IsHandled; }

	protected:
		bool m_IsHandled = false;
	};

	/*
		Helper class to handle dispatching events to the correct functions - and converting them to the right type.
	*/
	class EventDispatcher
	{
	public:
		/*
			Create on and event to be dispatched.
		*/
		EventDispatcher(IEvent& event)
			: m_Event(event)
		{}

		/*
			Function that does all the dispatching magic.
			Template it on the event type derived class, and pass a bound function that takes in a const reference to the derived class and returns a bool.
			For example:-
			bool OnWindowClose(const WindowCloseEvent& event)
			{
				return true;
			}

			You should return whether the event has been handled or not.
		*/
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

	/*
		Easier way to define the event type in a derived class.
	*/
	#define DEFINETYPES(eventType) virtual EventType GetType() override { return eventType; } \
								   static EventType GetStaticType() { return eventType; } 
}
