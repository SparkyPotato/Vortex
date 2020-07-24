#pragma once
#include <bitset>
#include <Core/IModule.h>
#include <Core/Events/InputEvent.h>

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

	/*
		Vortex Module that handles all Input Events.
	*/
	class VXInput : public IModule
	{
	public:
		friend class VXCore;

		VXInput();
		virtual ~VXInput();

		/*
			Does nothing yet.
		*/
		virtual int Startup() override;
		virtual int Shutdown() override;
		/*
			Resets the scroll delta every frame.
		*/
		virtual void Tick(float deltaTime) override;

		/*
			Does nothing yet.
		*/
		virtual void Quit() override;
		virtual void BlockQuit() override;
		virtual void AllowQuit() override;

		/*
			Get the current mouse state.
		*/
		const MouseState& GetMouseState() { return m_MouseState; }
		/*
			Is the key specified currently held down?
		*/
		bool IsKeyDown(InputCode keyCode) { return m_KeyStates[(int) keyCode]; }

	private:
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
	};
}
