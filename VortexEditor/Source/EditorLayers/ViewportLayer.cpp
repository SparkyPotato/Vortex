#include <EditorLayers/ViewportLayer.h>
#include <Private/Platforms/DirectX11/DX11Texture.h>
#include <Private/Platforms/DirectX11/DX11Framebuffer.h>
#include <World/World.h>
#include <Math/Matrix.h>

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
	m_Texture = GPTexture::Create(400, 400);
	m_Framebuffer = GPFramebuffer::Create(m_Texture);

	GRenderer->RenderToFramebuffer(m_Framebuffer);
}

void ViewportLayer::OnDetach()
{
	GRenderer->RenderToWindow(GWindow);

	delete m_Framebuffer;
}

void ViewportLayer::Tick(float deltaTime)
{
	if (!*m_IsOpen) return;

	HandleResize((int) m_ViewportSize.x, (int) m_ViewportSize.y);
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
			m_ViewportSize = ImGui::GetContentRegionAvail();

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
		GRenderer->ResizeFramebuffer(width, height);
	}
}
