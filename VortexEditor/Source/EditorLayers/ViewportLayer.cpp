#include <EditorLayers/ViewportLayer.h>
#include <Private/Platforms/DirectX11/DX11Texture.h>
#include <Private/Platforms/DirectX11/DX11Framebuffer.h>
#include <EditorLayers/WorldLayer.h>

using namespace Vortex;

DEFINE_LOGGER(LogViewport);

ViewportLayer::ViewportLayer(bool* isViewportCurrentlyOpen, WorldLayer* worldLayer)
	: m_IsOpen(isViewportCurrentlyOpen), m_WorldLayer(worldLayer)
{
	CREATE_LOGGER(LogViewport, spdlog::level::trace);

	m_EditorEntity = m_WorldLayer->GetEditorEntity();
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
	if (!*m_IsOpen || !m_IsMouseInViewportBounds) return;

	auto transform = m_EditorEntity->GetTransform();
	auto camera = m_EditorEntity->GetCameraComponent();

	if (GInput->GetMouseState().rightButton)
	{
		GWindow->HideCursor();

		m_MovementSpeed += GInput->GetMouseState().scrollDelta;
		if (m_MovementSpeed < 0.5f) m_MovementSpeed = 0.5f;

		if (GInput->IsKeyDown(InputCode::W))
			transform->SetPosition(transform->GetPosition() + camera->GetForwardVector() * m_MovementSpeed * deltaTime);
		if (GInput->IsKeyDown(InputCode::S))
			transform->SetPosition(transform->GetPosition() - camera->GetForwardVector() * m_MovementSpeed * deltaTime);
		if (GInput->IsKeyDown(InputCode::D))
			transform->SetPosition(transform->GetPosition() + camera->GetRightVector() * m_MovementSpeed * deltaTime);
		if (GInput->IsKeyDown(InputCode::A))
			transform->SetPosition(transform->GetPosition() - camera->GetRightVector() * m_MovementSpeed * deltaTime);
		if (GInput->IsKeyDown(InputCode::E))
			transform->SetPosition(transform->GetPosition() + Math::Vector(0.f, 1.f, 0.f) * m_MovementSpeed * deltaTime);
		if (GInput->IsKeyDown(InputCode::Q))
			transform->SetPosition(transform->GetPosition() - Math::Vector(0.f, 1.f, 0.f) * m_MovementSpeed * deltaTime);

		float yDelta = (float) GInput->GetMouseState().y - m_LastFrameMousePosition.y;
		float xDelta = (float) GInput->GetMouseState().x - m_LastFrameMousePosition.x;

		auto rotation = transform->GetRotation() + Math::Vector(-yDelta, -xDelta, 0.f) * m_Sensitivity / 5.f;

		if (rotation.x > 180.f)
			rotation.x = 180.f;
		else if (rotation.x < -180.f)
			rotation.x = -180.f;

		transform->SetRotation(rotation);
	}
	else
	{
		GWindow->ShowCursor();
	}

	m_LastFrameMousePosition.x = (float) GInput->GetMouseState().x;
	m_LastFrameMousePosition.y = (float) GInput->GetMouseState().y;
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
			auto size = ImGui::GetContentRegionAvail();
			
			HandleResize((int) size.x, (int) size.y);

			if (GraphicsContext::Get()->GetAPI() == GraphicsAPI::DirectX11)
			{
				DX11Texture* texture = reinterpret_cast<DX11Texture*>(m_Texture);
				ImGui::ImageButton(texture->GetShaderResource(), { (float) texture->GetWidth(), (float) texture->GetHeight() }, { 0, 0 }, { 1, 1 }, 0);

				m_ViewportTopLeft = ImGui::GetItemRectMin();
				m_ViewportBottomRight = ImGui::GetItemRectMax();

				if (ImGui::IsItemHovered())
					m_IsMouseInViewportBounds = true;
				else
					m_IsMouseInViewportBounds = false;
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
		GRenderer->RequestFramebufferResize(width, height);
	}
}
