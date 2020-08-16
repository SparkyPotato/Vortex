#include <EditorLayers/LogLayer.h>

using namespace Vortex;

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
	GInput->AddKeyBinding(BIND_INPUT(this->OpenConsole), InputCode::Tilde, Binding::Pressed);
	Logger::GetEditorSink()->SetLogCallback([&](Log& log) { if (log.level >= m_LogLevel) m_LogsToShow.push_back(log); });
	UpdateLog();
}

void LogLayer::OnDetach()
{
	GInput->RemoveKeyBinding(InputCode::Tilde, Binding::Pressed);
	Logger::GetEditorSink()->SetLogCallback(nullptr);
}

void LogLayer::Tick(float deltaTime)
{

}

void LogLayer::OnGuiRender()
{
	if (*m_IsOpen)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, { 800.f, 250.f });
		if (ImGui::Begin("Log", m_IsOpen), ImGuiWindowFlags_NoScrollbar)
		{
			const char* levels[] = { "Trace", "Debug", "Info", "Warn", "Error" };

			// Clear log button.
			if (ImGui::Button("Clear"))
			{
				Logger::GetEditorSink()->Clear();
				UpdateLog();
			}

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
					if (ImGui::Selectable(levels[n], isSelected) && n != m_LogLevel)
					{
						m_LogLevel = n;
						UpdateLog();
					}

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
				if (m_WasJustOpened)
				{
					ImGui::SetKeyboardFocusHere();
					m_WasJustOpened = false;
				}
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
	if (GInput->IsKeyDown(InputCode::Shift))
	{
		m_IsConsoleOpen = !m_IsConsoleOpen;
		m_WasJustOpened = true;
	}
}

void LogLayer::ShowLogText()
{
	ImGuiListClipper clipper;
	clipper.Begin((int) m_LogsToShow.size());

	while (clipper.Step())
	{
		for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
		{
			auto log = m_LogsToShow[i];

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
}

void LogLayer::UpdateLog()
{
	m_LogsToShow.clear();

	if (Logger::GetEditorSink()->GetLog().size() > 500)
	{
		Logger::GetEditorSink()->Shrink(250);
	}

	size_t size = Logger::GetEditorSink()->GetLog().size();
	for (int i = 0; i < size; i++)
	{
		Log log = Logger::GetEditorSink()->GetLog()[i];

		if (log.level >= m_LogLevel)
			m_LogsToShow.push_back(log);
	}
}
