#include <EditorApp.h>
#include <EditorLayers/LogLayer.h>
#include <EditorLayers/ViewportLayer.h>

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
	GLayerStack->PushLayer(new LogLayer(&m_IsLogOpen));
	GLayerStack->PushLayer(new ViewportLayer(&m_IsViewportOpen));
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

	ImGui::Begin("Main Window", &showMainWindow, window_flags);

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
			ImGui::MenuItem("Save", "Ctrl + S", selected, true);
			ImGui::Separator();
			if (ImGui::MenuItem("Exit", "", selected, true)) GCore->Quit();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window", true))
		{
			ImGui::MenuItem("Viewport", "", &m_IsViewportOpen, true);
			ImGui::Separator();
			ImGui::MenuItem("Log", "", &m_IsLogOpen, true);
			ImGui::MenuItem("Profiler", "", &m_ShowStats, true);

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	ImGui::End();

	// Draw profiler window.
	if (m_ShowStats)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, { 200.f, 100.f });
		if (ImGui::Begin("Profiler", &m_ShowStats))
		{
			ImGui::Text("Delta Time: %.3f ms", GCore->GetLastDeltaTime() * 1000.f);
			ImGui::Text("FPS: %.1f", 1 / GCore->GetLastDeltaTime());

			ImGui::End();
		}
		else
		{
			ImGui::End();
		}
		ImGui::PopStyleVar(1);
	}
}

void EditorApp::OnEvent(Vortex::IEvent& event)
{
	
}
