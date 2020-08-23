#pragma once
#include <Core/Module.h>
#include <Math/Vector.h>

DECLARE_LOGGER(Log3DUI);

namespace Vortex
{
	/*
		Module that deals with Im3d rendering.
	*/
	class VX3dUi : public Module
	{
	public:
		VX3dUi();
		virtual ~VX3dUi();

		virtual void Startup() override;
		virtual void Shutdown() override;
		
		virtual void Tick(float deltaTime) override;
		void Draw();

		virtual void Quit() override;

		virtual void OnConsoleCommand(ConsoleCommand command) override;

		void SetViewportTopLeft(const Math::Vector& topLeft) { m_ViewportTopLeft = topLeft; }

	private:
		Math::Vector m_ViewportTopLeft;
	};
}
