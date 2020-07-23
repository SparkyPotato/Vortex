#pragma once
#include <Core/IEvent.h>

namespace Vortex
{
	class WindowCloseEvent : public IEvent
	{
	public:
		WindowCloseEvent() {}
		
		DEFINETYPES(EventType::WindowClose);
	};

	class WindowMinimizeEvent : public IEvent
	{
	public:
		WindowMinimizeEvent() {}

		DEFINETYPES(EventType::WindowMinimize);
	};

	class WindowUnminimizeEvent : public IEvent
	{
	public:
		WindowUnminimizeEvent() {}

		DEFINETYPES(EventType::WindowUnminimize);
	};

	class WindowMaximizeEvent : public IEvent
	{
	public:
		WindowMaximizeEvent() {}

		DEFINETYPES(EventType::WindowMaximize);
	};

	class WindowUnmaximizeEvent : public IEvent
	{
	public:
		WindowUnmaximizeEvent() {}

		DEFINETYPES(EventType::WindowUnmaximize);
	};

	class WindowResizeEvent : public IEvent
	{
	public:
		WindowResizeEvent(int width, int height)
			: m_Width(width), m_Height(height)
		{}

		int GetWidth() { return m_Width; }
		int GetHeight() { return m_Height; }

		DEFINETYPES(EventType::WindowResize);

	private:
		int m_Width;
		int m_Height;
	};

	class WindowMoveEvent : public IEvent
	{
	public:
		WindowMoveEvent(int x, int y) 
			: m_X(x), m_Y(y)
		{}

		int GetXPosition() { return m_X; }
		int GetYPosition() { return m_Y; }

		DEFINETYPES(EventType::WindowMove);

	private:
		int m_X;
		int m_Y;
	};

	class WindowActivateEvent : public IEvent
	{
	public:
		WindowActivateEvent() {}

		DEFINETYPES(EventType::WindowActivate);
	};

	class WindowDeactivateEvent : public IEvent
	{
	public:
		WindowDeactivateEvent() {}

		DEFINETYPES(EventType::WindowDeactivate);
	};
}
