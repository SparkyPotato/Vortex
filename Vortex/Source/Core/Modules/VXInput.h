#pragma once
#include <bitset>
#include <Core/IModule.h>
#include <Core/Events/InputEvent.h>

namespace Vortex
{
	struct MouseState
	{
		int x = 0, y = 0;

		bool leftButton = false;
		bool rightButton = false;
		bool middleButton = false;
		bool x1Button = false;
		bool x2Button = false;

		float scrollDelta = 0.f;
	};

	class VXInput : public IModule
	{
	public:
		friend class VXCore;

		VXInput();
		virtual ~VXInput();

		virtual int Startup() override;
		virtual int Shutdown() override;
		virtual void Tick(float deltaTime) override;

		virtual void Quit() override;
		virtual void BlockQuit() override;
		virtual void AllowQuit() override;

		const MouseState& GetMouseState() { return m_MouseState; }
		bool IsKeyDown(InputCode keyCode) { return m_KeyStates[(int) keyCode]; }

	private:
		bool KDEvent(const KeyDownEvent& event);
		bool KUEvent(const KeyUpEvent& event);
		bool MMEvent(const MouseMoveEvent& event);
		bool MDEvent(const MouseButtonDownEvent& event);
		bool MUEvent(const MouseButtonUpEvent& event);
		bool MDCEvent(const MouseButtonDoubleClickEvent& event);
		bool MSEvent(const MouseScrollEvent& event);

		std::bitset<256> m_KeyStates;
		MouseState m_MouseState;
	};
}
