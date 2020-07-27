#include <EditorLayers/ViewportLayer.h>

using namespace Vortex;

ViewportLayer::ViewportLayer()
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
	bool* show = NULL;
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse;

	ImGui::Begin("Viewport", show, windowFlags);
	ImGui::End();
}
