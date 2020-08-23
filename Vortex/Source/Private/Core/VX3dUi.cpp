#include <VXpch.h>
#include <Core/Modules/VX3dUi.h>
#include <Core/VXConsole.h>
#include <Core/Modules/VXCore.h>
#include <World/World.h>

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
		m_VertexShaderTriangles = GPVertexShader::Create("../Vortex/Source/Graphics/Shaders/Im3d/VertexShaderTriangles.hlsl", "");
		m_VertexShaderNoTriangles = GPVertexShader::Create("../Vortex/Source/Graphics/Shaders/Im3d/VertexShaderNoTriangles.hlsl", "");

		m_GeometryShaderPoints = GPGeometryShader::Create("../Vortex/Source/Graphics/Shaders/Im3d/GeometryShaderPoints.hlsl", "");
		m_GeometryShaderLines = GPGeometryShader::Create("../Vortex/Source/Graphics/Shaders/Im3d/GeometryShaderLines.hlsl", "");

		m_PixelShaderPoints = GPPixelShader::Create("../Vortex/Source/Graphics/Shaders/Im3d/PixelShaderPoints.hlsl", "");
		m_PixelShaderLines = GPPixelShader::Create("../Vortex/Source/Graphics/Shaders/Im3d/PixelShaderLines.hlsl", "");
		m_PixelShaderTriangles = GPPixelShader::Create("../Vortex/Source/Graphics/Shaders/Im3d/PixelShaderTriangles.hlsl", "");

		m_ConstantBuffer = GPConstantBuffer::Create(&m_ConstantBufferData, sizeof(ConstantBufferData), (int) ConstantBufferTarget::VertexShader);

		m_GeometryConstantBuffer = GPConstantBuffer::Create(&m_GeometryConstantBufferData, sizeof(GeometryConstantBuffer), (int) ConstantBufferTarget::GeometryShader);

		auto& appData = Im3d::GetAppData();
		appData.m_flipGizmoWhenBehind = true;
	}

	void VX3dUi::Shutdown()
	{
		delete m_VertexShaderTriangles;
		delete m_VertexShaderNoTriangles;

		delete m_GeometryShaderPoints;
		delete m_GeometryShaderLines;

		delete m_PixelShaderPoints;
		delete m_PixelShaderLines;
		delete m_PixelShaderTriangles;

		delete m_ConstantBuffer;

		delete m_GeometryConstantBuffer;
	}

	void VX3dUi::Tick(float deltaTime)
	{
		VX_PROFILE("3D UI Render");

		auto& appData = Im3d::GetAppData();
		appData.m_deltaTime = deltaTime;

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

		m_ViewportSize = { (float)texture->GetWidth(), (float)texture->GetHeight(), 0.f };

		Math::Vector cursorPos = { (float) GInput->GetMouseState().x, (float) GInput->GetMouseState().y, 0.f };
		cursorPos -= m_ViewportTopLeft;

		if (cursorPos.x <= texture->GetWidth() && cursorPos.y <= texture->GetHeight() && cursorPos.x > 0.f && cursorPos.y > 0.f)
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
		auto drawLists = Im3d::GetDrawLists();
		unsigned int drawListCount = Im3d::GetDrawListCount();

		auto camera = GRenderer->GetWorld()->GetMainCamera();

		m_ConstantBufferData.viewProjectionMatrix = camera->GetViewProjectionMatrix();
		m_ConstantBuffer->Set(&m_ConstantBufferData);

		m_GeometryConstantBufferData.viewportDimensions = m_ViewportSize;
		m_GeometryConstantBuffer->Set(&m_GeometryConstantBufferData);

		for (unsigned int i = 0; i < drawListCount; i++)
		{
			const Im3d::DrawList& drawlist = drawLists[i];

			if (!m_Vertices || m_BufferSize < drawlist.m_vertexCount)
			{
				delete m_Vertices;

				m_Vertices = GPVertexBuffer::Create((void*) drawlist.m_vertexData, drawlist.m_vertexCount, m_VertexLayout, BufferAccessType::Dynamic);
				m_BufferSize = m_Vertices->GetSize();
			}
			else
			{
				m_Vertices->Set((void*) drawlist.m_vertexData, drawlist.m_vertexCount);
			}
			m_ConstantBuffer->Bind();
			m_GeometryConstantBuffer->Bind();

			switch (drawlist.m_primType)
			{
			case Im3d::DrawPrimitive_Triangles:
				GraphicsContext::Get()->SetTopology(Topology::TriangleList);
				m_VertexShaderTriangles->Bind();
				m_PixelShaderTriangles->Bind();
				break;
			case Im3d::DrawPrimitive_Lines:
				GraphicsContext::Get()->SetTopology(Topology::LineList);
				m_VertexShaderNoTriangles->Bind();
				m_GeometryShaderLines->Bind();
				m_PixelShaderLines->Bind();
				break;
			case Im3d::DrawPrimitive_Points:
				GraphicsContext::Get()->SetTopology(Topology::PointList);
				m_VertexShaderNoTriangles->Bind();
				m_GeometryShaderPoints->Bind();
				m_PixelShaderPoints->Bind();
				break;
			}

			m_Vertices->Bind();

			GraphicsContext::Get()->Draw(drawlist.m_vertexCount);

			m_VertexShaderTriangles->Unbind();
			m_GeometryShaderPoints->Unbind();
			m_PixelShaderPoints->Unbind();
		}
	}

	void VX3dUi::Quit()
	{

	}

	void VX3dUi::OnConsoleCommand(ConsoleCommand command)
	{

	}
}
