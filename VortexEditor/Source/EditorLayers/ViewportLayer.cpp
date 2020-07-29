#include <EditorLayers/ViewportLayer.h>
#include <Private/Platforms/DirectX11/DX11Texture.h>
#include <Private/Platforms/DirectX11/DX11Framebuffer.h>

using namespace Vortex;

ViewportLayer::ViewportLayer(bool* isViewportCurrentlyOpen)
	: m_IsOpen(isViewportCurrentlyOpen)
{
	
}

ViewportLayer::~ViewportLayer()
{
	
}

void ViewportLayer::OnAttach()
{
	m_Texture = GPTexture::Create(300, 300);
	m_Framebuffer = GPFramebuffer::Create(m_Texture);

	unsigned int indices[] = { 0, 1, 2 };
	m_IBuffer = GPIndexBuffer::Create(indices, 3);

	struct Vertex
	{
		float x, y, z;
		float r, g, b, a;
	};

	Vertex vertices[] =
	{
		{ -0.5f, -0.5f, 0.5f, 0.976f, 0.521f, 0.545f, 1.f },
		{ 0.f, 0.5f, 0.5f, 0.827f, 0.207f, 0.058f, 1.f },
		{ 0.5f, -0.5f, 0.5f, 0.38f, 0.074f, 0.027f, 1.f }
	};
	VertexLayout layout =
	{
		VertexElement("POSITION", ShaderDataType::float3),
		VertexElement("COLOR", ShaderDataType::float4)
	};

	m_VBuffer = GPVertexBuffer::Create(vertices, 3, layout);

	m_VShader = GPVertexShader::Create("../Vortex/Source/Graphics/Shaders/BasicVertexShader.hlsl");

	m_PShader = GPPixelShader::Create("../Vortex/Source/Graphics/Shaders/BasicPixelShader.hlsl");
}

void ViewportLayer::OnDetach()
{
	delete m_Framebuffer;
	delete m_VBuffer;
	delete m_IBuffer;

	delete m_VShader;
	delete m_PShader;
}

void ViewportLayer::Tick(float deltaTime)
{
	m_Framebuffer->Bind();
	m_Framebuffer->Clear(0, 0.5, 0.5, 1);

	m_VShader->Bind();
	m_PShader->Bind();

	m_VBuffer->Bind();
	m_IBuffer->Bind();

	GraphicsContext::Get()->Draw(3);
}

void ViewportLayer::OnGuiRender()
{
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;

	// Create viewport.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, { 400.f, 400.f });
	if (*m_IsOpen)
	{
		if (ImGui::Begin("Viewport", m_IsOpen, windowFlags))
		{
			HandleResize((int) ImGui::GetContentRegionAvail().x, (int) ImGui::GetContentRegionAvail().y);

			if (GraphicsContext::Get()->GetAPI() == GraphicsAPI::DirectX11)
			{
				DX11Texture* texture = reinterpret_cast<DX11Texture*>(m_Texture);
				ImGui::Image((void*)texture->GetShaderResource(), { (float) texture->GetWidth(), (float) texture->GetHeight() });
			}

			ImGui::End();
		}
		else
		{
			ImGui::End();
		}
	}
	ImGui::PopStyleVar(1);
}

void ViewportLayer::HandleResize(int width, int height)
{
	if (width <= 0 || height <= 0)
		return;
	else if (width != m_Texture->GetWidth() || height != m_Texture->GetHeight())
	{
		m_Framebuffer->Resize(width, height);
	}
}

