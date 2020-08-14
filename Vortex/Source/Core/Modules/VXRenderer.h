#pragma once
#include <Core/Module.h>
#include <Graphics/Primitives/GPFramebuffer.h>
#include <Core/Events/WindowEvent.h>

namespace Vortex
{
	class World;

	class VXRenderer : public Module
	{
	public:
		VXRenderer();
		virtual ~VXRenderer();

		virtual void Startup() override;
		virtual void Shutdown() override;
		
		virtual void Tick(float deltaTime) override;

		virtual void Quit() override;

		void RenderToFramebuffer(GPFramebuffer* framebuffer);
		void RenderToWindow(Window* window);

		void ResizeFramebuffer(int width, int height);
		bool ResizeOnWindow(WindowResizeEvent& event);

		void RenderWorld(World* worldToRender);

	private:
		void PostFrambufferResize(int width, int height);

		bool m_IsOnWindow;
		GPFramebuffer* m_Target;

		World* m_World;
	};
}

extern Vortex::VXRenderer* GRenderer;
