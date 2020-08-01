#pragma once
#include <Core/Module.h>

namespace Vortex
{
	class VXRenderer : public Module
	{
	public:
		VXRenderer();
		virtual ~VXRenderer();

		virtual void Startup() override;
		virtual void Shutdown() override;
		
		virtual void Tick(float deltaTime) override;

		virtual void Quit() override;
	};
}
