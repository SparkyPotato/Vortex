#include <VXpch.h>
#include <Core/Modules/VXInput.h>

namespace Vortex
{
	int VXInput::Startup()
	{
		return 0;
	}

	int VXInput::Shutdown()
	{
		return 0;
	}

	void VXInput::Tick(float deltaTime)
	{
		m_MouseState.scrollDelta = 0.f;
	}

	void VXInput::Quit()
	{

	}

	void VXInput::BlockQuit()
	{

	}

	void VXInput::AllowQuit()
	{

	}

	bool VXInput::KDEvent(const KeyDownEvent& event)
	{
		m_KeyStates[event.GetKeyCode()] = 1;
		return true;
	}

	bool VXInput::KUEvent(const KeyUpEvent& event)
	{
		m_KeyStates[event.GetKeyCode()] = 0;
		return true;
	}

	bool VXInput::MMEvent(const MouseMoveEvent& event)
	{
		m_MouseState.x = event.GetX();
		m_MouseState.y = event.GetY();
		return true;
	}

	bool VXInput::MDEvent(const MouseButtonDownEvent& event)
	{
		switch (event.GetButton())
		{
		case (int) InputCode::LeftButton:
			m_MouseState.leftButton = true;
			break;
		case (int) InputCode::RightButton:
			m_MouseState.rightButton = true;
			break;
		case (int) InputCode::MiddleButton:
			m_MouseState.middleButton = true;
			break;
		default:
			ENG_WARN("Illegal mouse event.");
		}
		return true;
	}

	bool VXInput::MUEvent(const MouseButtonUpEvent& event)
	{
		switch (event.GetButton())
		{
		case (int)InputCode::LeftButton:
			m_MouseState.leftButton = false;
			break;
		case (int)InputCode::RightButton:
			m_MouseState.rightButton = false;
			break;
		case (int)InputCode::MiddleButton:
			m_MouseState.middleButton = false;
			break;
		default:
			ENG_WARN("Illegal mouse event.");
		}
		return true;
	}

	bool VXInput::MDCEvent(const MouseButtonDoubleClickEvent& event)
	{
		return true;
	}

	bool VXInput::MSEvent(const MouseScrollEvent& event)
	{
		m_MouseState.scrollDelta = event.GetScrollSteps();
		return true;
	}

	VXInput::VXInput()
	{

	}

	VXInput::~VXInput()
	{

	}
}
