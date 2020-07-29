#pragma once
#include <Core/Module.h>

namespace Vortex
{
	class VXCore;
	class Window;

	class VXGui : public Module
	{
	public:
		VXGui(VXCore* core);
		virtual ~VXGui();

		virtual void Startup() override;
		virtual void Shutdown() override;

		virtual void Tick(float deltaTime) override;

		virtual void Quit() override;
		virtual void BlockQuit() override;
		virtual void AllowQuit() override;

	private:
		VXCore* GCore;

		Window* m_Window;
	};
}
