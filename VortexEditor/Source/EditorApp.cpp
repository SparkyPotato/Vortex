#include "EditorApp.h"

using namespace Vortex;

IApplication* CreateApplication()
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
	GCore->GetLayerStack()->PushLayer(new ViewportLayer(&m_IsViewportOpen));
}

void EditorApp::Tick(float deltaTime)
{
	m_DeltaTime = deltaTime;
}

void EditorApp::OnGuiRender()
{
	// Poll for shortcuts.

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
			bool* selected = NULL;
			ImGui::MenuItem("New", "Ctrl + N", selected, true);
			ImGui::MenuItem("Open", "Ctrl + O", selected, true);
			ImGui::Separator();
			if (ImGui::MenuItem("Quit", "Ctrl + Q", selected, true)) GCore->Quit();

			ImGui::EndMenu();
		} 
		if (ImGui::BeginMenu("Window", true))
		{
			ImGui::MenuItem("Viewport", "Alt + V", &m_IsViewportOpen, true);
			ImGui::Separator();
			ImGui::MenuItem("Statistics", "", &m_ShowStats, true);

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	ImGui::End();

	// Draw statistics window
	if (m_ShowStats)
	{
		ImGui::SetNextWindowSize({ 200.f, 100.f });
		if (ImGui::Begin("Statistics", &m_ShowStats, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking))
		{
			ImGui::Text("Delta Time: %f ms", m_DeltaTime * 1000.f);

			ImGui::End();
		}
		else
		{
			ImGui::End();
		}
	}
}

void EditorApp::OnEvent(Vortex::IEvent& event)
{
	
}
