#pragma once
#include <Core/IEvent.h>

namespace Vortex
{
	/*
		Generated on window close.
	*/
	class WindowCloseEvent : public IEvent
	{
	public:
		WindowCloseEvent() {}
		
		DEFINETYPES(EventType::WindowClose);
	};

	/*
		Generated on window minimize.
	*/
	class WindowMinimizeEvent : public IEvent
	{
	public:
		WindowMinimizeEvent() {}

		DEFINETYPES(EventType::WindowMinimize);
	};

	/*
		Generated on window unminimize.
	*/
	class WindowUnminimizeEvent : public IEvent
	{
	public:
		WindowUnminimizeEvent() {}

		DEFINETYPES(EventType::WindowUnminimize);
	};

	/*
		Generated on window maximize.
	*/
	class WindowMaximizeEvent : public IEvent
	{
	public:
		WindowMaximizeEvent() {}

		DEFINETYPES(EventType::WindowMaximize);
	};

	/*
		Generated on window unmaximize.
	*/
	class WindowUnmaximizeEvent : public IEvent
	{
	public:
		WindowUnmaximizeEvent() {}

		DEFINETYPES(EventType::WindowUnmaximize);
	};

	/*
		Generated on window resize.
	*/
	class WindowResizeEvent : public IEvent
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
	class WindowMoveEvent : public IEvent
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
	class WindowActivateEvent : public IEvent
	{
	public:
		WindowActivateEvent() {}

		DEFINETYPES(EventType::WindowActivate);
	};

	/*
		Generated on window focus loss.
	*/
	class WindowDeactivateEvent : public IEvent
	{
	public:
		WindowDeactivateEvent() {}

		DEFINETYPES(EventType::WindowDeactivate);
	};
}
