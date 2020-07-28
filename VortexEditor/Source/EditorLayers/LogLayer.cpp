#include <EditorLayers/LogLayer.h>

using namespace Vortex;

LogLayer::LogLayer(bool* isLogCurrentlyOpen)
	: m_IsOpen(isLogCurrentlyOpen)
{

}

LogLayer::~LogLayer()
{

}

void LogLayer::OnAttach()
{

}

void LogLayer::OnDetach()
{

}

void LogLayer::Tick(float deltaTime)
{

}

void LogLayer::OnGuiRender()
{
	if (*m_IsOpen)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, { 400.f, 150.f });
		if (ImGui::Begin("Log", m_IsOpen))
		{
			const char* levels[] = { "Trace", "Info", "Warn", "Error" };

			// Engine log level dropdown.
			if (ImGui::BeginCombo("Engine Log Level", levels[m_EngineLogLevel]))
			{
				for (int n = 0; n < 4; n++)
				{
					const bool isSelected = (m_EngineLogLevel == n);
					if (ImGui::Selectable(levels[n], isSelected))
						m_EngineLogLevel = n;

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			// Client log level dropdown.
			if (ImGui::BeginCombo("Client Log Level", levels[m_ClientLogLevel]))
			{
				for (int n = 0; n < 4; n++)
				{
					const bool isSelected = (m_ClientLogLevel == n);
					if (ImGui::Selectable(levels[n], isSelected))
						m_ClientLogLevel = n;

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			// Clear log button.
			if (ImGui::Button("Clear")) Logger::GetEditorSink()->Clear();

			// Puts log text in a seprate scrollable window.
			ImGui::BeginChild("Scroll");

			// Shows log text.
			ShowLogText();

			if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.f);

			ImGui::EndChild();

			ImGui::End();
		}
		else
		{
			ImGui::End();
		}
		
		ImGui::PopStyleVar(1);
	}
}

void LogLayer::ShowLogText()
{
	std::vector<const EditorSink_mt::Log*> logs;
	for (size_t i = 0; i < Logger::GetEditorSink()->GetLog().size(); i++)
	{
		const EditorSink_mt::Log* log = &(Logger::GetEditorSink()->GetLog()[i]);

		if (log->location == EditorSink_mt::LogLoc::Engine && (int) log->level >= m_EngineLogLevel)
			logs.push_back(log);
		else if (log->location == EditorSink_mt::LogLoc::Client && (int) log->level >= m_ClientLogLevel)
			logs.push_back(log);
	}

	ImGuiListClipper clipper;
	clipper.Begin((int) logs.size());

	while (clipper.Step())
	{
		for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
		{
			auto log = *logs[i];

			if (log.level == EditorSink_mt::LogLevel::Trace)
				ImGui::TextColored({ 0.7f, 0.7f, 0.7f, 1.f }, log.message.c_str());
			else if (log.level == EditorSink_mt::LogLevel::Info)
				ImGui::TextColored({ 0.f, 1.f, 0.f, 1.f }, log.message.c_str());
			else if (log.level == EditorSink_mt::LogLevel::Warn)
				ImGui::TextColored({ 1.f, 1.f, 0.f, 1.f }, log.message.c_str());
			else
				ImGui::TextColored({ 1.f, 0.f, 0.f, 1.f }, log.message.c_str());
		}
	}

	clipper.End();
}
