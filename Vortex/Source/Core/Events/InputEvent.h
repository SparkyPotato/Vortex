#pragma once
#include <Core/InputCode.h>
#include <Core/IEvent.h>

namespace Vortex
{
	class KeyEvent : public IEvent
	{
	public:
		int GetKeyCode() const { return m_Code; }

	protected:
		KeyEvent(int keyCode)
		: m_Code(keyCode)
		{}

		int m_Code;
	};

	class KeyDownEvent : public KeyEvent
	{
	public:
		KeyDownEvent(int keyCode)
			: KeyEvent(keyCode)
		{}

		DEFINETYPES(EventType::KeyDown);
	};

	class KeyUpEvent : public KeyEvent
	{
	public:
		KeyUpEvent(int keyCode)
			: KeyEvent(keyCode)
		{}

		DEFINETYPES(EventType::KeyUp);
	};

	class MouseMoveEvent : public IEvent
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

	class MouseButtonDownEvent : public IEvent
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

	class MouseButtonUpEvent : public IEvent
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

	class MouseButtonDoubleClickEvent : public IEvent
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

	class MouseScrollEvent : public IEvent
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
