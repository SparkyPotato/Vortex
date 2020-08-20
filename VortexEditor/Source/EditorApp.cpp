#include <fstream>
#include <filesystem>
#include <EditorApp.h>
#include <EditorLayers/LogLayer.h>
#include <EditorLayers/ViewportLayer.h>
#include <EditorLayers/ProfilerLayer.h>
#include <EditorLayers/WorldLayer.h>
#include <EditorLayers/AssetLayer.h>

using namespace Vortex;

DEFINE_LOGGER(LogEditor);

Application* CreateApplication()
{
	CREATE_LOGGER(LogEditor, spdlog::level::trace);
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
	LoadLayout(m_LayoutFilePath);

	GLayerStack->PushLayer(new ViewportLayer(&m_IsViewportOpen));
	GLayerStack->PushLayer(new WorldLayer(&m_IsWorldOpen, &m_IsPropertiesOpen));
	GLayerStack->PushLayer(new AssetLayer(&m_IsAssetManagerOpen));
	GLayerStack->PushLayer(new LogLayer(&m_IsLogOpen));
	GLayerStack->PushLayer(new ProfilerLayer(&m_IsProfilerOpen));
}

void EditorApp::Quit()
{
	SaveLayout(m_LayoutFilePath);
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

	// Create a Dockspace in the window.
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
			ImGui::MenuItem("World", "", &m_IsWorldOpen, true);
			ImGui::MenuItem("Properties", "", &m_IsPropertiesOpen, true);
			ImGui::MenuItem("Asset Manager", "", &m_IsAssetManagerOpen, true);
			ImGui::Separator();
			ImGui::MenuItem("Log", "", &m_IsLogOpen, true);
			ImGui::MenuItem("Profiler", "", &m_IsProfilerOpen, true);

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	ImGui::End();
}

void EditorApp::OnEvent(Vortex::Event& event)
{
	if (event.GetType() == EventType::WindowClose)
	{
		m_IsClosing = true;
	}
}

void EditorApp::OnConsoleCommand(Vortex::ConsoleCommand command)
{
	if (command.command == "loadlayout")
	{
		VX_INFO(LogConsole, "Loading Layout.");
		LoadLayout(m_LayoutFilePath);
		VXConsole::Get()->SubmitCommand("gui.restart");
	}
	else if (command.command == "help")
	{
		VX_INFO(LogConsole, "Editor: Functions: ");
		VX_INFO(LogConsole, "Editor:     loadlayout - Loads the layout of the last application startup.");
	}
	else
	{
		VX_ERROR(LogConsole, "'{0}' is not a valid Editor command!", command.command);
	}
}

void EditorApp::LoadLayout(std::string file)
{
	std::ifstream prefsFile(file);

	VX_INFO(LogEditor, "Loading Editor layout.");

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
			m_IsProfilerOpen = true;
		else
			m_IsProfilerOpen = false;

		getline(prefsFile, line);
		line.erase(0, 11);
		if (line == "1")
			m_IsWorldOpen = true;
		else
			m_IsWorldOpen = false;

		getline(prefsFile, line);
		line.erase(0, 10);
		if (line == "1")
			m_IsPropertiesOpen = true;
		else	   
			m_IsPropertiesOpen = false;

		getline(prefsFile, line);
		line.erase(0, 11);
		if (line == "1")
			m_IsAssetManagerOpen = true;
		else
			m_IsAssetManagerOpen = false;

		prefsFile.close();
	}
	else
	{
		VX_WARN(LogEditor, "Couldn't open preferences file. Using default settings.");
		auto path = std::filesystem::path("Preferences/");
		std::filesystem::create_directory(path);
	}
}

void EditorApp::SaveLayout(std::string file)
{
	VX_INFO(LogEditor, "Saving Editor layout.");

	std::ofstream prefsFile(file);

	if (prefsFile.is_open())
	{
		prefsFile << "vpstate " << m_IsViewportOpen << "\n";
		prefsFile << "logstate " << m_IsLogOpen << "\n";
		prefsFile << "profstate " << m_IsProfilerOpen << "\n";
		prefsFile << "worldstate " << m_IsWorldOpen << "\n";
		prefsFile << "propstate " << m_IsPropertiesOpen << "\n";
		prefsFile << "assetstate " << m_IsAssetManagerOpen << "\n";

		prefsFile.close();
	}
	else
	{
		VX_ERROR(LogEditor, "Failed to write to preferences file! Not saving preferences.");
	}
}
