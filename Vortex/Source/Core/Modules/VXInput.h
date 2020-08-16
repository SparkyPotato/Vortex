#pragma once
#include <bitset>
#include <Core/Module.h>
#include <Core/Events/InputEvent.h>

DECLARE_LOGGER(LogInput);

namespace Vortex
{
	/*
		Describes the state of the mouse.
	*/
	struct MouseState
	{
		// Position of the mouse in window-space.
		int x = 0, y = 0;

		// Button states.
		bool leftButton = false;
		bool rightButton = false;
		bool middleButton = false;
		bool x1Button = false;
		bool x2Button = false;

		// Scroll delta from the last frame.
		float scrollDelta = 0.f;
	};

	enum class Binding
	{
		Pressed,
		Released
	};

	/*
		Vortex Module that handles all Input Events.
	*/
	class VXInput : public Module
	{
	public:
		friend class VXCore;

		VXInput();
		virtual ~VXInput();

		/*
			Does nothing yet.
		*/
		virtual void Startup() override;
		virtual void Shutdown() override;
		/*
			Resets the scroll delta every frame.
		*/
		virtual void Tick(float deltaTime) override;

		/*
			Does nothing yet.
		*/
		virtual void Quit() override;

		virtual void OnConsoleCommand(ConsoleCommand command) override;

		/*
			Get the current mouse state.
		*/
		const MouseState& GetMouseState() { return m_MouseState; }
		/*
			Is the key specified currently held down?
		*/
		bool IsKeyDown(InputCode keyCode) { return m_KeyStates[(int) keyCode]; }

		void AddKeyBinding(std::function<void(void)> keyFunction, std::string functionName, InputCode key, Binding bindingType);
		void AddKeyBinding(std::function<void(void)> keyFunction, InputCode key, Binding bindingType);

		void RemoveKeyBinding(InputCode key, Binding bindingType);

		void ClearInputState();

	private:
		struct Binder
		{
			Binder(std::function<void(void)> func, std::string funcName, InputCode iCode, Binding bnding)
				: function(func), functionName(funcName), code(iCode), binding(bnding)
			{}

			Binder(std::function<void(void)> func, InputCode iCode, Binding bnding)
				: function(func), functionName("unset"), code(iCode), binding(bnding)
			{}

			bool operator==(const Binder& other)
			{
				return (code == other.code && binding == other.binding);
			}

			std::function<void(void)> function;
			std::string functionName;
			InputCode code;
			Binding binding;
		};

		/*
			Messily named functions that deal with getting events from the Vortex Core.
		*/
		bool KDEvent(const KeyDownEvent& event);
		bool KUEvent(const KeyUpEvent& event);
		bool MMEvent(const MouseMoveEvent& event);
		bool MDEvent(const MouseButtonDownEvent& event);
		bool MUEvent(const MouseButtonUpEvent& event);
		bool MDCEvent(const MouseButtonDoubleClickEvent& event);
		bool MSEvent(const MouseScrollEvent& event);

		// Key states and mouse state.
		std::bitset<256> m_KeyStates;
		MouseState m_MouseState;

		std::vector<Binder> m_Binders;
	};
}

// Global pointer to the Input.
extern Vortex::VXInput* GInput;
