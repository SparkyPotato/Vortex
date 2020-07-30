#include <EditorLayers/ProfilerLayer.h>

using namespace Vortex;

ProfilerLayer::ProfilerLayer(bool* isProfilerOpen)
	: m_IsOpen(isProfilerOpen)
{

}

ProfilerLayer::~ProfilerLayer()
{

}

void ProfilerLayer::OnAttach()
{

}

void ProfilerLayer::OnDetach()
{

}

void ProfilerLayer::Tick(float deltaTime)
{
	m_CurrentDeltaTime = 1000.f * deltaTime;
}

void ProfilerLayer::OnGuiRender()
{
	if (*m_IsOpen)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, { 300.f, 200.f });
		if (ImGui::Begin("Profiler", m_IsOpen))
		{
			if (!m_IsPaused)
			{
				if (ImGui::Button("Pause###Pause"))
				{
					m_IsPaused = true;
					m_PausedDeltaTime = m_CurrentDeltaTime;
					m_PausedProfiles = Profiler::GetProfiles();
				}

				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::TextUnformatted("Reduce GUI usage to get a more accurate reading.");
					ImGui::EndTooltip();
				}

				ImGui::Text("Delta Time: %.3f ms", m_CurrentDeltaTime);
				ImGui::Text("FPS: %.1f", 1000.f / m_CurrentDeltaTime);

				ImGui::End();
			}
			else
			{
				if (ImGui::Button("Continue###Pause"))
				{
					m_IsPaused = false;
				}

				ImGui::Text("Delta Time: %.3f ms", m_PausedDeltaTime);
				ImGui::Text("FPS: %.1f", 1000.f / m_PausedDeltaTime);

				ImGui::Separator();

				for (auto profile : m_PausedProfiles)
				{
					if (profile.second >= m_PausedDeltaTime / 2.f)
						ImGui::TextColored({ 0.5f, 0.f, 0.f, 1.f }, "%s: %f ms", profile.first.c_str(), profile.second);
					else if (profile.second >= m_PausedDeltaTime / 4.f)
						ImGui::TextColored({ 1.f, 0.f, 0.f, 1.f }, "%s: %f ms", profile.first.c_str(), profile.second);
					else if (profile.second >= m_PausedDeltaTime / 5.f)
						ImGui::TextColored({ 0.f, 1.f, 1.f, 1.f }, "%s: %f ms", profile.first.c_str(), profile.second);
					else if (profile.second >= m_PausedDeltaTime / 10.f)
						ImGui::TextColored({ 0.f, 1.f, 0.f, 1.f }, "%s: %f ms", profile.first.c_str(), profile.second);
					else
						ImGui::TextColored({ 1.f, 1.f, 1.f, 1.f }, "%s: %f ms", profile.first.c_str(), profile.second);
				}

				ImGui::End();
			}
		}
		else
		{
			ImGui::End();
		}
		ImGui::PopStyleVar(1);
	}
}
