#include <EditorApp.h>
#include <fstream>
#include <filesystem>
#include <EditorLayers/LogLayer.h>
#include <EditorLayers/ViewportLayer.h>

using namespace Vortex;

IApplication* CreateApplication()
{
	return new EditorApp();
}

EditorApp::EditorApp()
{
	LoadPrefs("Preferences/Editor.vxprefs");
}

EditorApp::~EditorApp()
{
	SavePrefs("Preferences/Editor.vxprefs");
}

void EditorApp::Start()
{
	GLayerStack->PushLayer(new ViewportLayer(&m_IsViewportOpen));
	GLayerStack->PushLayer(new LogLayer(&m_IsLogOpen));
}

void EditorApp::Tick(float deltaTime)
{
	
}

void EditorApp::OnGuiRender()
{
	//Create main ImGui window.
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
			ImGui::MenuItem("Profiler", "", &m_ShowProfiler, true);

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	ImGui::End();

	// Draw profiler window.
	if (m_ShowProfiler)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, { 200.f, 100.f });
		if (ImGui::Begin("Profiler", &m_ShowProfiler))
		{
			ImGui::Text("Delta Time: %.3f ms", GCore->GetLastDeltaTime() * 1000.f);
			ImGui::Text("FPS: %.1f", 1 / GCore->GetLastDeltaTime());

			ImGui::Separator();

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
	if (event.GetType() == EventType::WindowClose)
	{
		m_IsClosing = true;
	}
}

void EditorApp::LoadPrefs(std::string file)
{
	std::ifstream prefsFile(file);

	if (prefsFile.is_open())
	{
		auto temp = std::filesystem::path("Temp/Log.txt");

		std::string line;

		getline(prefsFile, line);
		line.erase(0, 8);
		if (line == "1")
			m_IsViewportOpen = true;
		else
			m_IsViewportOpen = false;

		getline(prefsFile, line);
		line.erase(0, 9);
		if (line == "1")
			m_IsLogOpen = true;
		else
			m_IsLogOpen = false;

		getline(prefsFile, line);
		line.erase(0, 10);
		if (line == "1")
			m_ShowProfiler = true;
		else
			m_ShowProfiler = false;

		prefsFile.close();
	}
	else
	{
		VX_WARN("Couldn't open preferences file. Using default settings...");
		auto path = std::filesystem::path("Preferences/");
		std::filesystem::create_directory(path);
	}
}

void EditorApp::SavePrefs(std::string file)
{
	std::ofstream prefsFile(file);

	if (prefsFile.is_open())
	{
		prefsFile << "vpstate " << m_IsViewportOpen << "\n";
		prefsFile << "logstate " << m_IsLogOpen << "\n";
		prefsFile << "profstate " << m_ShowProfiler << "\n";

		prefsFile.close();
	}
	else
	{
		VX_ERROR("Failed to write to preferences file! Not saving preferences.");
	}
}
