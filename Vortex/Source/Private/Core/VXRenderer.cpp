#include <VXpch.h>
#include <Core/Modules/VXRenderer.h>

Vortex::VXRenderer* GRenderer;

namespace Vortex
{
	VXRenderer::VXRenderer()
	{
		
	}

	VXRenderer::~VXRenderer()
	{

	}

	void VXRenderer::Startup()
	{
		m_Target = GWindow->GetFramebuffer();
		m_IsOnWindow = true;

		::GRenderer = this;
	}

	void VXRenderer::Shutdown()
	{

	}

	void VXRenderer::Tick(float deltaTime)
	{
		VX_PROFILE("Rendering");

		m_Target->Bind();
		m_Target->Clear(0.f, 0.5f, 0.5f, 1.f);
	}

	void VXRenderer::Quit()
	{

	}

	void VXRenderer::RenderToFramebuffer(GPFramebuffer* framebuffer)
	{
		m_IsOnWindow = false;
		m_Target = framebuffer;
	}

	void VXRenderer::RenderToWindow(Window* window)
	{
		m_IsOnWindow = true;
		m_Target = window->GetFramebuffer();
	}

	void VXRenderer::ResizeFramebuffer(int width, int height)
	{
		if (m_IsOnWindow)
		{
			ENG_ERROR("Cannot manually resize the Renderer on a Window!");
			return;
		}

		m_Target->Resize(width, height);
		PostFrambufferResize(width, height);
	}

	bool VXRenderer::ResizeOnWindow(WindowResizeEvent& event)
	{
		if (!m_IsOnWindow) return false;

		PostFrambufferResize(event.GetWidth(), event.GetHeight());
		return false;
	}

	void VXRenderer::RenderWorld(World* worldToRender)
	{
		m_World = worldToRender;
	}

	void VXRenderer::PostFrambufferResize(int width, int height)
	{

	}
}
