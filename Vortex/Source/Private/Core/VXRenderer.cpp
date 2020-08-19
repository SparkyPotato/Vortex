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
		m_ColorPixelShader = GPPixelShader::Create("../Vortex/Source/Graphics/Shaders/ColorPixelShader.hlsl");
		m_TexturePixelShader = GPPixelShader::Create("../Vortex/Source/Graphics/Shaders/TexturePixelShader.hlsl");

		m_BasicVertexShader->Bind();
	}

	void VXRenderer::Shutdown()
	{
		delete m_ConstantBuffer;
		delete m_BasicVertexShader;
		delete m_ColorPixelShader;
		delete m_TexturePixelShader;
	}

	void VXRenderer::Tick(float deltaTime)
	{
		VX_PROFILE("Rendering");

		if (!m_World) return;

		m_Target->Bind();
		m_Target->Clear(0.f, 0.5f, 0.5f, 1.f);

		auto camera = m_World->GetMainCamera();
		if (!camera) return;

		m_ConstantBuffer->Bind();

		for (auto sprite : m_World->GetSprites())
		{
 			m_ConstantBufferData.worldViewProjectionMatrix = sprite->GetTransform()* camera->GetViewProjectionMatrix();

			m_ConstantBuffer->Set(&m_ConstantBufferData);

			sprite->GetQuad().vertices->Bind();
			sprite->GetQuad().indices->Bind();

			if (sprite->GetTexture())
			{
				m_TexturePixelShader->Bind();
				sprite->GetTexture()->Bind();
			}
			else
			{
				m_ColorPixelShader->Bind();
			}

			GraphicsContext::Get()->Draw(sprite->GetQuad().indices->GetSize());
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

	void VXRenderer::RequestFramebufferResize(int width, int height)
	{
		m_Width = width;
		m_Height = height;
		m_ShouldResize = true;
	}

	void VXRenderer::ResizeIfRequired()
	{
		if (m_IsOnWindow) return;

		if (m_ShouldResize)
		{
			ResizeFramebuffer(m_Width, m_Height);
			m_ShouldResize = false;
		}
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
		for (auto camera : m_World->GetCameras())
		{
			camera->Resize((float)width / (float)height);
		}
	}
}
