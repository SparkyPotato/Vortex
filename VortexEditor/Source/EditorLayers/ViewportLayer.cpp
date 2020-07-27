#include <EditorLayers/ViewportLayer.h>

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

}

void ViewportLayer::OnDetach()
{

}

void ViewportLayer::Tick(float deltaTime)
{

}

void ViewportLayer::OnGuiRender()
{
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse;

	// Create viewport.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, { 400.f, 400.f });
	ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.f, 0.f, 0.f, 1.f });
	if (*m_IsOpen)
	{
		ImGui::Begin("Viewport", m_IsOpen, windowFlags);

		if (IGraphicsContext::Get()->GetAPI() == GraphicsAPI::DirectX11)
		{
			ImGui::Text("The viewport doesn't really do anything yet...");
		}

		ImGui::End();
	}
	ImGui::PopStyleVar(1);
	ImGui::PopStyleColor(1);
}
