#pragma once
#include <Core/Module.h>

namespace Vortex
{
	class VXCore;
	class Window;

	/*
		Module that deals with ImGui rendering.
	*/
	class VXGui : public Module
	{
	public:
		VXGui();
		virtual ~VXGui();

		virtual void Startup() override;
		virtual void Shutdown() override;

		virtual void Tick(float deltaTime) override;

		virtual void Quit() override;
		virtual void BlockQuit() override;
		virtual void AllowQuit() override;

	private:
		Window* m_Window;
	};
}
