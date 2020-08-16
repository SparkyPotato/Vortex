#include <VXpch.h>
#include <Core/Modules/VXInput.h>
#include <Core/VXConsole.h>

Vortex::VXInput* GInput;

namespace Vortex
{
	CREATE_LOGGER_LOCAL(LogInput, spdlog::level::trace);

	void VXInput::Startup()
	{
		VX_TRACE(LogInput, "Started Vortex Input Module.");

		::GInput = this;
	}

	void VXInput::Shutdown()
	{
		VX_TRACE(LogInput, "Shutdown Vortex Input Module.");
	}

	void VXInput::Tick(float deltaTime)
	{
		// Reset scroll delta.
		m_MouseState.scrollDelta = 0.f;
	}

	void VXInput::Quit()
	{

	}

	void VXInput::OnConsoleCommand(ConsoleCommand command)
	{
		
	}

	void VXInput::AddKeyBinding(std::function<void(void)> keyFunction, InputCode key, Binding bindingType)
	{
		m_Binders.emplace_back(keyFunction, key, bindingType);
	}

	void VXInput::ClearInputState()
	{
		for (int i = 0; i < 256; i++)
		{
			m_KeyStates[i] = false;
		}

		m_MouseState.leftButton = false;
		m_MouseState.rightButton = false;
		m_MouseState.middleButton = false;
		m_MouseState.x1Button = false;
		m_MouseState.x2Button = false;
	}

	bool VXInput::KDEvent(const KeyDownEvent& event)
	{
		// Set the bit to true.
		m_KeyStates[event.GetKeyCode()] = true;

		// Call bound functions.
		for (auto& binder : m_Binders)
		{
			if (binder.code == (InputCode) event.GetKeyCode() && binder.binding == Binding::Pressed)
			{
				binder.function();
			}
		}

		return true;
	}

	bool VXInput::KUEvent(const KeyUpEvent& event)
	{
		// Set the bit to false.
		m_KeyStates[event.GetKeyCode()] = false;

		// Call bound functions.
		for (auto& binder : m_Binders)
		{
			if (binder.code == (InputCode) event.GetKeyCode() && binder.binding == Binding::Released)
			{
				binder.function();
			}
		}

		return true;
	}

	bool VXInput::MMEvent(const MouseMoveEvent& event)
	{
		// Set the mouse position.
		m_MouseState.x = event.GetX();
		m_MouseState.y = event.GetY();
		return true;
	}

	bool VXInput::MDEvent(const MouseButtonDownEvent& event)
	{
		// Set the correct button down based on what was pressed.
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
			VX_WARN(LogInput, "Illegal mouse event.");
		}
		return true;
	}

	bool VXInput::MUEvent(const MouseButtonUpEvent& event)
	{
		// Set the correct button up based on what was pressed.
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
			VX_WARN(LogInput, "Illegal mouse event.");
		}
		return true;
	}

	bool VXInput::MDCEvent(const MouseButtonDoubleClickEvent& event)
	{
		// Does nothing yet.
		return true;
	}

	bool VXInput::MSEvent(const MouseScrollEvent& event)
	{
		// Sets the scroll delta.
		m_MouseState.scrollDelta += event.GetScrollSteps();
		return true;
	}

	VXInput::VXInput()
	{

	}

	VXInput::~VXInput()
	{

	}
}
