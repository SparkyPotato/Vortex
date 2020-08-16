#include <EditorLayers/ViewportLayer.h>
#include <Private/Platforms/DirectX11/DX11Texture.h>
#include <Private/Platforms/DirectX11/DX11Framebuffer.h>
#include <World/World.h>
#include <Math/Matrix.h>

using namespace Vortex;

ViewportLayer::ViewportLayer(bool* isViewportCurrentlyOpen)
	: m_IsOpen(isViewportCurrentlyOpen)
{
	m_World = new Vortex::World;

	m_EditorEntity = m_World->CreateEntity("Editor Entity");
	m_EditorCamera = m_EditorEntity->AddCameraComponent();
	m_EditorCamera->SetCameraSettings(CameraProjection::Perspective, 1.f, 0.5f, 1000.f);
}

ViewportLayer::~ViewportLayer()
{
	delete m_World;
}

void ViewportLayer::OnAttach()
{
	m_Texture = GPTexture::Create(400, 400);
	m_Framebuffer = GPFramebuffer::Create(m_Texture);

	m_EditorEntity->GetTransform().SetPosition({ 0.f, 0.f, 0.f });

	auto sprite = m_World->CreateEntity("Sprite");
	sprite->AddSpriteComponent(2.f, 2.f);
	sprite->GetTransform().SetPosition({ 0.f, 0.f, 1.f });

	GRenderer->RenderToFramebuffer(m_Framebuffer);
	GRenderer->RenderWorld(m_World);
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
