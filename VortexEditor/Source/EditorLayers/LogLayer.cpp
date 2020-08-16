#include <EditorLayers/LogLayer.h>
#include <Core/VXConsole.h>

using namespace Vortex;

DEFINE_LOGGER(LogDefault, spdlog::level::trace);

LogLayer::LogLayer(bool* isLogCurrentlyOpen)
	: m_IsOpen(isLogCurrentlyOpen)
{
	memset(m_ConsoleBuffer, 0, sizeof(m_ConsoleBuffer));
}

LogLayer::~LogLayer()
{

}

void LogLayer::OnAttach()
{
	GInput->AddKeyBinding([&]() { this->OpenConsole(); }, InputCode::Tilde, Binding::Pressed);
}

void LogLayer::OnDetach()
{

}

void LogLayer::Tick(float deltaTime)
{

}

void LogLayer::OnGuiRender()
{
	bool show = true;
	ImGui::ShowDemoWindow(&show);

	if (*m_IsOpen)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, { 800.f, 250.f });
		if (ImGui::Begin("Log", m_IsOpen))
		{
			m_IsFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

			const char* levels[] = { "Trace", "Debug", "Info", "Warn", "Error" };

			// Clear log button.
			if (ImGui::Button("Clear")) Logger::GetEditorSink()->Clear();

			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::TextUnformatted("Clears the output");
				ImGui::EndTooltip();
			}
			ImGui::SameLine();

			// Engine log level dropdown.
			if (ImGui::BeginCombo("Log Level", levels[m_LogLevel]))
			{
				for (int n = 0; n < 5; n++)
				{
					const bool isSelected = (m_LogLevel == n);
					if (ImGui::Selectable(levels[n], isSelected))
						m_LogLevel = n;

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			// Puts log text in a separate scrollable window.
			if (m_IsConsoleOpen)
			{
				ImGui::BeginChild("Scroll", { 0.f, -ImGui::GetFontSize() * 2.2f });
			}
			else
			{
				ImGui::BeginChild("Scroll");
			}

			// Shows log text.
			ShowLogText();

			if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.f);

			ImGui::EndChild();

			if (m_IsConsoleOpen)
			{
				ImGui::Separator();

				ImGui::PushItemWidth(-1);
				if (ImGui::InputTextWithHint("##ConsoleInput", "Console", m_ConsoleBuffer, sizeof(m_ConsoleBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
				{
					VXConsole::Get()->SubmitCommand(m_ConsoleBuffer);

					ImGui::SetKeyboardFocusHere();
					memset(m_ConsoleBuffer, 0, sizeof(m_ConsoleBuffer));
				}
				ImGui::PopItemWidth();
			}

			ImGui::End();
		}
		else
		{
			ImGui::End();
		}
		
		ImGui::PopStyleVar(1);
	}
}

void LogLayer::OpenConsole()
{
	if (m_IsFocused && GInput->IsKeyDown(InputCode::Shift))
	{
		m_IsConsoleOpen = !m_IsConsoleOpen;
	}
}

void LogLayer::ShowLogText()
{
	Logger::GetEditorSink()->Shrink(250);

	size_t size = Logger::GetEditorSink()->GetLog().size();
	for (int i = 0; i < size; i++)
	{
		const Log* log = &(Logger::GetEditorSink()->GetLog()[i]);

		if (log->level >= m_LogLevel)
			m_Logs.push_back(log);
	}

	ImGuiListClipper clipper;
	clipper.Begin((int) m_Logs.size());

	while (clipper.Step())
	{
		for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
		{
			auto log = *m_Logs[i];

			if (log.level == spdlog::level::trace)
				ImGui::TextColored({ 0.7f, 0.7f, 0.7f, 1.f }, log.message.c_str());
			else if (log.level == spdlog::level::debug)
				ImGui::TextColored({ 1.f, 1.f, 1.f, 1.f }, log.message.c_str());
			else if (log.level == spdlog::level::info)
				ImGui::TextColored({ 0.f, 1.f, 0.f, 1.f }, log.message.c_str());
			else if (log.level == spdlog::level::warn)
				ImGui::TextColored({ 1.f, 1.f, 0.f, 1.f }, log.message.c_str());
			else
				ImGui::TextColored({ 1.f, 0.f, 0.f, 1.f }, log.message.c_str());
		}
	}

	clipper.End();

	m_Logs.clear();
}
