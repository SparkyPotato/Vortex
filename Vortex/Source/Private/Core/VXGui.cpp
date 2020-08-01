#include <VXpch.h>
#include <Core/Modules/VXGui.h>
#include <Core/Modules/VXCore.h>
#include <Graphics/Primitives/GPFramebuffer.h>
#include <Private/Core/ImGuiImplementation.h>
#include <imgui.h>

namespace Vortex
{
	void VXGui::Startup()
	{
		// Setup ImGui.
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		// io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.DisplaySize = ImVec2((float) GCore->GetWindow()->GetProperties().width, (float) GCore->GetWindow()->GetProperties().height);
		io.IniFilename = "Preferences/UILayout.vxprefs";

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.f;
			style.Colors[ImGuiCol_WindowBg].w = 1.f;
		}

		// Set the window style.
		style.ChildRounding = 0.f;
		style.GrabRounding = 3.f;
		style.WindowRounding = 3.f;
		style.ScrollbarRounding = 3.f;
		style.FrameRounding = 3.f;
		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

		// Set the colors used.
		ImVec4* colors = style.Colors;

		colors[ImGuiCol_WindowBg] = { 0.f, 0.f, 0.f, 1.f };

		colors[ImGuiCol_TitleBg] = { 0.f, 0.f, 0.f, 1.f };
		colors[ImGuiCol_TitleBgActive] = { 0.1f, 0.1f, 0.1f, 1.f };

		colors[ImGuiCol_DockingEmptyBg] = { 0.f, 0.f, 0.f, 1.f };
		colors[ImGuiCol_MenuBarBg] = { 0.f, 0.f, 0.f, 1.f };

		colors[ImGuiCol_PopupBg] = { 0.1f, 0.1f, 0.1f, 1.f };
		colors[ImGuiCol_Border] = { 0.1f, 0.1f, 0.1f, 1.f };

		colors[ImGuiCol_Separator] = { 0.2f, 0.2f, 0.2f, 1.f };
		colors[ImGuiCol_SeparatorHovered] = { 0.2f, 0.2f, 0.35f, 1.f };
		colors[ImGuiCol_SeparatorActive] = { 0.25f, 0.282f, 0.788f, 1.f };

		colors[ImGuiCol_ResizeGrip] = { 0.1f, 0.1f, 0.1f, 0.5f };
		colors[ImGuiCol_ResizeGripHovered] = { 0.2f, 0.2f, 0.35f, 1.f };
		colors[ImGuiCol_ResizeGripActive] = { 0.25f, 0.282f, 0.788f, 1.f };

		colors[ImGuiCol_ScrollbarBg] = { 0.1f, 0.1f, 0.1f, 0.5f };
		colors[ImGuiCol_ScrollbarGrab] = { 0.2f, 0.2f, 0.2f, 1.f };
		colors[ImGuiCol_ScrollbarGrabHovered] = { 0.2f, 0.2f, 0.35f, 1.f };
		colors[ImGuiCol_ScrollbarGrabActive] = { 0.25f, 0.282f, 0.788f, 1.f };

		colors[ImGuiCol_Tab] = { 0.2f, 0.2f, 0.2f, 1.f };
		colors[ImGuiCol_TabUnfocused] = { 0.2f, 0.2f, 0.2f, 1.f };
		colors[ImGuiCol_TabUnfocusedActive] = { 0.2f, 0.2f, 0.35f, 1.f };
		colors[ImGuiCol_TabHovered] = { 0.25f, 0.282f, 0.788f, 1.f };
		colors[ImGuiCol_TabActive] = { 0.313f, 0.352f, 0.988f, 1.f };

		colors[ImGuiCol_Header] = { 0.2f, 0.2f, 0.25f, 1.f };
		colors[ImGuiCol_HeaderHovered] = { 0.2f, 0.2f, 0.35f, 1.f };
		colors[ImGuiCol_HeaderActive] = { 0.25f, 0.282f, 0.788f, 1.f };

		colors[ImGuiCol_FrameBg] = { 0.25f, 0.25f, 0.25f, 1.f };
		colors[ImGuiCol_FrameBgHovered] = { 0.2f, 0.2f, 0.35f, 1.f };
		colors[ImGuiCol_FrameBgActive] = { 0.25f, 0.282f, 0.788f, 1.f };

		colors[ImGuiCol_Button] = { 0.3f, 0.3f, 0.3f, 1.f };
		colors[ImGuiCol_ButtonHovered] = { 0.25f, 0.282f, 0.788f, 1.f };
		colors[ImGuiCol_ButtonActive] = { 0.313f, 0.352f, 0.988f, 1.f };

		// Initializes the ImGui backend.
		ImGuiImplementation::Init(GCore->GetWindow());
	}

	void VXGui::Shutdown()
	{
		// Shuts down the ImGui backend.
		ImGuiImplementation::Shutdown();
		ImGui::DestroyContext();
	}

	void VXGui::Tick(float deltaTime)
	{
		// Profiles GUI Render.
		ENG_PROFILE("GUI Render");

		// Bind main window framebuffer.
		m_Window->GetFramebuffer()->Bind();

		// Start ImGui frame.
		ImGuiImplementation::NewFrame();
		ImGui::NewFrame();

		// Call all GUI Render functions.
		GCore->GetApplication()->OnGuiRender();
		GLayerStack->RenderGui();

		// Render ImGui.
		ImGui::Render();
		ImGuiImplementation::Draw(ImGui::GetDrawData());

		// Update windows for ImGui viewports.
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	void VXGui::Quit()
	{
		
	}

	void VXGui::BlockQuit()
	{

	}

	void VXGui::AllowQuit()
	{

	}

	VXGui::VXGui()
	{
		m_Window = GWindow;
	}

	VXGui::~VXGui()
	{

	}
}
