#pragma once
#include <Core/IModule.h>

namespace Vortex
{
	class VXCore;
	class IWindow;

	class VXGui : public IModule
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

		IWindow* m_Window;
	};
}
