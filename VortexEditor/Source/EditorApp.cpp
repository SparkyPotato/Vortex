#include "EditorApp.h"

Vortex::IApplication* CreateApplication()
{
	return new EditorApp();
}

EditorApp::EditorApp()
{
	
}

EditorApp::~EditorApp()
{
	
}

void EditorApp::Start()
{
	GCore->GetLayerStack()->PushLayer(new ViewportLayer());
}

void EditorApp::Tick(float deltaTime)
{
	
}

void EditorApp::OnGuiRender()
{
	// Create main ImGui window.
	bool showMainWindow = true;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | 
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->GetWorkPos());
	ImGui::SetNextWindowSize(viewport->GetWorkSize());
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("Dock", &showMainWindow, window_flags);

	ImGui::PopStyleVar(3);

	// Create a dockspace in the window.
	ImGuiID dockspace_id = ImGui::GetID("Vortex Editor Dock");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

	// Creating the Menu Bar.
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File", true))
		{
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	ImGui::End();
}

void EditorApp::OnEvent(Vortex::IEvent& event)
{
	
}
