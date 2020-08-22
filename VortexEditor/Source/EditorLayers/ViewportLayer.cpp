#include <EditorLayers/ViewportLayer.h>
#include <Private/Platforms/DirectX11/DX11Texture.h>
#include <Private/Platforms/DirectX11/DX11Framebuffer.h>
#include <EditorLayers/WorldLayer.h>
#include <im3d.h>
#include <im3d_math.h>

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
	GCore->SetCustomPostRender([&](float deltaTime) {this->Im3dRenderer(deltaTime); });

	GRenderer->RenderToFramebuffer(m_Framebuffer);
}

void ViewportLayer::OnDetach()
{
	GRenderer->RenderToWindow(GWindow);
	GCore->SetCustomPostRender(nullptr);

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
		GWindow->TrapCursor
		(
			(int) m_ViewportTopLeft.x, (int) m_ViewportTopLeft.y, 
			(int) m_ViewportBottomRight.x, (int) m_ViewportBottomRight.y
		);
		GInput->UseRawInput(true);

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

		if (rotation.x > 90.f)
			rotation.x = 90.f;
		else if (rotation.x < -90.f)
			rotation.x = -90.f;

		transform->SetRotation(rotation);
	}
	else if (GInput->GetMouseState().leftButton)
	{
		// Selecting stuff.
	}
	else
	{
		GWindow->ShowCursor();
		GWindow->FreeCursor();
		GInput->UseRawInput(false);
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
				auto texture = reinterpret_cast<DX11Texture*>(m_Texture);
				ImGui::Image(texture->GetShaderResource(), { (float) texture->GetWidth(), (float) texture->GetHeight() });

				m_ViewportTopLeft = ImGui::GetItemRectMin();
				m_ViewportBottomRight = ImGui::GetItemRectMax();

				m_IsMouseInViewportBounds = ImGui::IsItemHovered();
				m_IsViewportActive = ImGui::IsWindowFocused();
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

void ViewportLayer::Im3dRenderer(float deltaTime)
{
	Im3dNewFrame(deltaTime);

	Im3d::PushDrawState();
	Im3d::SetSize(2.0f);
	Im3d::BeginLineLoop();
	Im3d::Vertex(0.0f, 0.0f, 0.0f, Im3d::Color_Magenta);
	Im3d::Vertex(1.0f, 1.0f, 0.0f, Im3d::Color_Yellow);
	Im3d::Vertex(2.0f, 2.0f, 0.0f, Im3d::Color_Cyan);
	Im3d::End();
	Im3d::PopDrawState();

	Im3dEndFrame();
}

void ViewportLayer::Im3dNewFrame(float deltaTime)
{
	auto& appData = Im3d::GetAppData();

	Math::Vector direction = m_EditorEntity->GetCameraComponent()->GetForwardVector();

	appData.m_deltaTime = deltaTime;
	appData.m_viewportSize = { (float) m_Texture->GetWidth(), (float) m_Texture->GetHeight() };
	appData.m_viewOrigin = m_EditorEntity->GetTransform()->GetPosition();
	appData.m_viewDirection = direction;
	appData.m_worldUp = { 0.f, 1.f, 0.f };
	appData.m_projOrtho = false;

	float atanFOV = m_EditorEntity->GetCameraComponent()->GetProjectionMatrix().columns[0].x;
	appData.m_projScaleY = 2 * atanFOV / 1 - atanFOV * atanFOV;

	Im3d::Vec2 cursorPos = { GInput->GetMouseState().x - m_ViewportTopLeft.x, GInput->GetMouseState().y - m_ViewportTopLeft.y };
	cursorPos = (cursorPos / appData.m_viewportSize) * 2.0f - 1.0f;
	cursorPos.y = -cursorPos.y;

	Im3d::Vec3 rayOrigin, rayDirection;
	rayOrigin = appData.m_viewOrigin;
	rayDirection.x = cursorPos.x / m_EditorEntity->GetCameraComponent()->GetProjectionMatrix().columns[0].x;
	rayDirection.y = cursorPos.y / m_EditorEntity->GetCameraComponent()->GetProjectionMatrix().columns[1].y;
	rayDirection.z = -1.0f;
	rayDirection = direction;

	appData.m_cursorRayOrigin = rayOrigin;
	appData.m_cursorRayDirection = rayDirection;

	appData.m_keyDown[Im3d::Key::Action_Select] = GInput->GetMouseState().leftButton;
	appData.m_keyDown[Im3d::Key::Action_GizmoTranslation] = GInput->IsKeyDown(InputCode::W) && m_IsViewportActive && !GInput->GetMouseState().rightButton;
	appData.m_keyDown[Im3d::Key::Action_GizmoRotation] = GInput->IsKeyDown(InputCode::E) && m_IsViewportActive && !GInput->GetMouseState().rightButton;
	appData.m_keyDown[Im3d::Key::Action_GizmoScale] = GInput->IsKeyDown(InputCode::R) && m_IsViewportActive && !GInput->GetMouseState().rightButton;

	appData.m_snapTranslation = GInput->IsKeyDown(InputCode::LeftControl) ? 1.f : 0.f;
	appData.m_snapRotation = GInput->IsKeyDown(InputCode::LeftControl) ? Im3d::Radians(15.f) : 0.f;
	appData.m_snapScale = GInput->IsKeyDown(InputCode::LeftControl) ? 0.25f : 0.f;

	Im3d::NewFrame();
}

void ViewportLayer::Im3dEndFrame()
{
	Im3d::EndFrame();

	GRenderer->DrawIm3d(Im3d::GetDrawListCount(), Im3d::GetDrawLists());
}
