#pragma once
#include <Core/Event.h>

namespace Vortex
{
	/*
		Generated on window close.
	*/
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}
		
		DEFINETYPES(EventType::WindowClose);
	};

	/*
		Generated on window minimize.
	*/
	class WindowMinimizeEvent : public Event
	{
	public:
		WindowMinimizeEvent() {}

		DEFINETYPES(EventType::WindowMinimize);
	};

	/*
		Generated on window unminimize.
	*/
	class WindowUnminimizeEvent : public Event
	{
	public:
		WindowUnminimizeEvent() {}

		DEFINETYPES(EventType::WindowUnminimize);
	};

	/*
		Generated on window maximize.
	*/
	class WindowMaximizeEvent : public Event
	{
	public:
		WindowMaximizeEvent() {}

		DEFINETYPES(EventType::WindowMaximize);
	};

	/*
		Generated on window unmaximize.
	*/
	class WindowUnmaximizeEvent : public Event
	{
	public:
		WindowUnmaximizeEvent() {}

		DEFINETYPES(EventType::WindowUnmaximize);
	};

	/*
		Generated on window resize.
	*/
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int width, int height)
			: m_Width(width), m_Height(height)
		{}

		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }

		DEFINETYPES(EventType::WindowResize);

	private:
		int m_Width;
		int m_Height;
	};

	/*
		Generated on window move.
	*/
	class WindowMoveEvent : public Event
	{
	public:
		WindowMoveEvent(int x, int y) 
			: m_X(x), m_Y(y)
		{}

		int GetXPosition() const { return m_X; }
		int GetYPosition() const { return m_Y; }

		DEFINETYPES(EventType::WindowMove);

	private:
		int m_X;
		int m_Y;
	};

	/*
		Generated on window focus gain.
	*/
	class WindowActivateEvent : public Event
	{
	public:
		WindowActivateEvent() {}

		DEFINETYPES(EventType::WindowActivate);
	};

	/*
		Generated on window focus loss.
	*/
	class WindowDeactivateEvent : public Event
	{
	public:
		WindowDeactivateEvent() {}

		DEFINETYPES(EventType::WindowDeactivate);
	};
}
