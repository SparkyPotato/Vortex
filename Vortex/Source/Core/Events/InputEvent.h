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
		Generated on key release. Handles by Vortex Input Module.
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
		Generated on mouse move. Handles by Vortex Input Module.
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
		Generated on mouse button press. Handles by Vortex Input Module.
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
		Generated on mouse button release. Handles by Vortex Input Module.
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
		Generated on mouse button double click. Handles by Vortex Input Module.
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
		Generated on mouse scroll. Handles by Vortex Input Module.
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
}
