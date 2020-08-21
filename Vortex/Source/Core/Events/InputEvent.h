#pragma once
#include <Core/InputCode.h>
#include <Core/Event.h>

namespace Vortex
{
	/*
		Base class for key events. Stores a key code.
	*/
	class KeyEvent : public Event
	{
	public:
		int GetKeyCode() const { return m_Code; }

	protected:
		KeyEvent(int keyCode)
		: m_Code(keyCode)
		{}

		int m_Code;
	};

	/*
		Generated on key press. Handled by Vortex Input Module.
	*/
	class KeyDownEvent : public KeyEvent
	{
	public:
		KeyDownEvent(int keyCode)
			: KeyEvent(keyCode)
		{}

		DEFINETYPES(EventType::KeyDown);
	};

	/*
		Generated on key release. Handled by Vortex Input Module.
	*/
	class KeyUpEvent : public KeyEvent
	{
	public:
		KeyUpEvent(int keyCode)
			: KeyEvent(keyCode)
		{}

		DEFINETYPES(EventType::KeyUp);
	};

	/*
		Generated on mouse move. Handled by Vortex Input Module.
	*/
	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(int x, int y)
			: m_X(x), m_Y(y)
		{}

		int GetX() const { return m_X; }
		int GetY() const { return m_Y; }

		DEFINETYPES(EventType::MouseMove);

	private:
		int m_X;
		int m_Y;
	};

	/*
		Generated on mouse button press. Handled by Vortex Input Module.
	*/
	class MouseButtonDownEvent : public Event
	{
	public:
		MouseButtonDownEvent(int mouseButton)
			: m_MouseButton(mouseButton)
		{}

		int GetButton() const { return m_MouseButton; }

		DEFINETYPES(EventType::MouseButtonDown);

	private:
		int m_MouseButton;
	};

	/*
		Generated on mouse button release. Handled by Vortex Input Module.
	*/
	class MouseButtonUpEvent : public Event
	{
	public:
		MouseButtonUpEvent(int mouseButton)
			: m_MouseButton(mouseButton)
		{}

		int GetButton() const { return m_MouseButton; }

		DEFINETYPES(EventType::MouseButtonUp);

	private:
		int m_MouseButton;
	};

	/*
		Generated on mouse button double click. Handled by Vortex Input Module.
	*/
	class MouseButtonDoubleClickEvent : public Event
	{
	public:
		MouseButtonDoubleClickEvent(int mouseButton)
			: m_MouseButton(mouseButton)
		{}

		int GetButton() const { return m_MouseButton; }

		DEFINETYPES(EventType::MouseButtonDoubleClick);

	private:
		int m_MouseButton;
	};

	/*
		Generated on mouse scroll. Handled by Vortex Input Module.
	*/
	class MouseScrollEvent : public Event
	{
	public:
		MouseScrollEvent(float scrollSteps)
			: m_ScrollSteps(scrollSteps)
		{}

		float GetScrollSteps() const { return m_ScrollSteps; }

		DEFINETYPES(EventType::MouseScroll);

	private:
		float m_ScrollSteps;
	};

	/*
		Generated on mouse Raw Input even. Handled by Vortex Input Module.
	*/
	class MouseRawEvent : public Event
	{
	public:
		MouseRawEvent(int deltaX, int deltaY)
			: m_DeltaX(deltaX), m_DeltaY(deltaY)
		{}

		int GetXDelta() const { return m_DeltaX; }
		int GetYDelta() const { return m_DeltaY; }

		DEFINETYPES(EventType::MouseRaw);

	private:
		int m_DeltaX, m_DeltaY;
	};
}
