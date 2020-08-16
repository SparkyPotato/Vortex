#include <VXpch.h>
#include <Core/Modules/VXRenderer.h>
#include <World/World.h>
#include <Core/VXConsole.h>

Vortex::VXRenderer* GRenderer;

DEFINE_LOGGER(LogRenderer);

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
		CREATE_LOGGER(LogRenderer, spdlog::level::trace);

		m_Target = GWindow->GetFramebuffer();
		m_IsOnWindow = true;

		ConstantBuffer buffer;
		m_ConstantBuffer = GPConstantBuffer::Create(&buffer, sizeof(ConstantBuffer), ConstantBufferTarget::VertexShader);
		m_BasicVertexShader = GPVertexShader::Create("../Vortex/Source/Graphics/Shaders/BasicVertexShader.hlsl");
		m_BasicPixelShader = GPPixelShader::Create("../Vortex/Source/Graphics/Shaders/BasicPixelShader.hlsl");

		m_BasicVertexShader->Bind();
		m_BasicPixelShader->Bind();

		::GRenderer = this;
	}

	void VXRenderer::Shutdown()
	{
		delete m_ConstantBuffer;
		delete m_BasicVertexShader;
		delete m_BasicPixelShader;
	}

	void VXRenderer::Tick(float deltaTime)
	{
		VX_PROFILE("Rendering");

		m_Target->Bind();
		m_Target->Clear(0.f, 0.5f, 0.5f, 1.f);

		auto camera = m_World->GetCameras().begin();
		m_ConstantBuffer->Bind();

		for (auto& sprite : m_World->GetSprites())
		{
// 			m_ConstantBufferData.worldViewProjectionMatrix 
// 				= sprite.GetQuad().transformation * camera->GetViewProjectionMatrix();

			m_ConstantBufferData.worldViewProjectionMatrix = Math::Matrix();
			m_ConstantBuffer->Set(&m_ConstantBufferData);

			sprite.GetQuad().vertices->Bind();
			sprite.GetQuad().indices->Bind();

			GraphicsContext::Get()->Draw(sprite.GetQuad().indices->GetSize());
		}
	}

	void VXRenderer::Quit()
	{

	}

	void VXRenderer::OnConsoleCommand(ConsoleCommand command)
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
			VX_ERROR(LogRenderer, "Cannot manually resize the Renderer on a Window!");
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
		auto camera = m_World->GetCameras().begin();
		camera->Resize((float) width / (float) height);
	}
}
