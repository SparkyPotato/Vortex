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

		ImGuiImplementation::Init(GCore->GetWindow());
	}

	void VXGui::Shutdown()
	{
		ImGuiImplementation::Shutdown();
		ImGui::DestroyContext();
	}

	void VXGui::Tick(float deltaTime)
	{
		m_Window->GetFramebuffer()->Bind();

		ImGuiImplementation::NewFrame();
		ImGui::NewFrame();

		GCore->GetApplication()->OnGuiRender();
		GCore->GetLayerStack()->RenderGui();

		ImGui::Render();
		ImGuiImplementation::Draw(ImGui::GetDrawData());

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

	VXGui::VXGui(VXCore* core)
	{
		GCore = core;
		m_Window = core->GetWindow();
	}

	VXGui::~VXGui()
	{

	}
}
