#include <VXpch.h>
#include <Core/Modules/VX3dUi.h>
#include <Core/VXConsole.h>
#include <Core/Modules/VXCore.h>
#include <World/World.h>
#include <im3d.h>

DEFINE_LOGGER(Log3DUI);

namespace Vortex
{
	VX3dUi::VX3dUi()
	{
		CREATE_LOGGER(Log3DUI, spdlog::level::trace);
	}

	VX3dUi::~VX3dUi()
	{

	}

	void VX3dUi::Startup()
	{

	}

	void VX3dUi::Shutdown()
	{

	}

	void VX3dUi::Tick(float deltaTime)
	{
		VX_PROFILE("3D UI Render");

		auto& appData = Im3d::GetAppData();
		appData.m_deltaTime = deltaTime;
		appData.m_flipGizmoWhenBehind = false;

		auto camera = GRenderer->GetWorld()->GetMainCamera();

		appData.m_projOrtho = camera->GetProjectionMode() == CameraProjection::Orthographic;
		if (appData.m_projOrtho)
			appData.m_projScaleY = 2.f / camera->GetProjectionMatrix().columns[1].y;
		else
			appData.m_projScaleY = 1.f / camera->GetProjectionMatrix().columns[1].y;
		appData.m_worldUp = { 0.f, 1.f, 0.f };
		Math::Vector pos = camera->GetTransform()->GetWorldPosition();

		appData.m_viewOrigin = { pos.x, pos.y, pos.z };
		appData.m_viewDirection = { camera->GetForwardVector().x, camera->GetForwardVector().y, camera->GetForwardVector().z };

		auto texture = GRenderer->GetTarget()->GetTexture();
		appData.m_viewportSize = { (float) texture->GetWidth(), (float) texture->GetHeight() };
		appData.m_keyDown[Im3d::Action_Select] = GInput->GetMouseState().leftButton;

		Math::Vector cursorPos = { (float) GInput->GetMouseState().x, (float) GInput->GetMouseState().y, 0.f };
		cursorPos -= m_ViewportTopLeft;

		if (cursorPos.x <= texture->GetWidth() && cursorPos.y <= texture->GetHeight() && cursorPos.x < 0.f && cursorPos.y < 0.f)
		{
			cursorPos.x = (cursorPos.x / appData.m_viewportSize.x) * 2.0f - 1.0f;
			cursorPos.y = -(cursorPos.y / appData.m_viewportSize.y) * 2.0f + 1.0f;

			Math::Vector rayOrigin, rayDirection;
			if (appData.m_projOrtho)
			{
				rayOrigin.x = cursorPos.x / camera->GetProjectionMatrix().columns[0].x;
				rayOrigin.y = cursorPos.y / camera->GetProjectionMatrix().columns[1].y;
				rayOrigin.z = 0.f;
				rayOrigin *= camera->GetTransform()->GetTransform();
				rayDirection = camera->GetForwardVector();
			}
			else
			{
				rayOrigin = pos;
				rayDirection.x = cursorPos.x / camera->GetProjectionMatrix().columns[0].x;
				rayDirection.y = cursorPos.y / camera->GetProjectionMatrix().columns[1].y;
				rayDirection.z = 1.f;
				rayDirection.w = 0.f;

				rayDirection *= camera->GetNearPlane();
				rayDirection = rayDirection.GetNormalized() * camera->GetTransform()->GetRotationMatrix();
			}
			rayDirection.Normalize();

			appData.m_cursorRayOrigin = { rayOrigin.x, rayOrigin.y, rayOrigin.z };
			appData.m_cursorRayDirection = { rayDirection.x, rayDirection.y, rayDirection.z };
		}

		Im3d::NewFrame();

		GCore->GetApplication()->On3dUiRender();
		GLayerStack->Render3dUi();

		Im3d::EndFrame();
	}

	void VX3dUi::Draw()
	{
		
	}

	void VX3dUi::Quit()
	{

	}

	void VX3dUi::OnConsoleCommand(ConsoleCommand command)
	{

	}
}
