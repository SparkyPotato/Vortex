#include <EditorLayers/ConsoleLayer.h>
#include <COre/VXConsole.h>

using namespace Vortex;


ConsoleLayer::ConsoleLayer(bool* isConsoleCurrentlyOpen)
	: m_IsOpen(isConsoleCurrentlyOpen)
{
	memset(m_ConsoleBuffer, 0, sizeof(m_ConsoleBuffer));
}

ConsoleLayer::~ConsoleLayer()
{

}

void ConsoleLayer::OnAttach()
{

}

void ConsoleLayer::OnDetach()
{

}

void ConsoleLayer::Tick(float deltaTime)
{

}

void ConsoleLayer::OnGuiRender()
{
	if (*m_IsOpen)
	{
		if (ImGui::Begin("Console", m_IsOpen))
		{
			ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags_EnterReturnsTrue;
			if (ImGui::InputTextWithHint("", "Enter command", m_ConsoleBuffer, sizeof(m_ConsoleBuffer), inputFlags))
			{
				ImGui::SetKeyboardFocusHere();

				if (std::string(m_ConsoleBuffer) == "clear")
				{
					m_Commands.clear();
				}
				else
				{
					m_Commands.push_back(m_ConsoleBuffer);
					if (!VXConsole::Get()->SubmitCommand(m_ConsoleBuffer))
					{
						m_Commands.push_back("Invalid command!");
					}
				}
				memset(m_ConsoleBuffer, 0, sizeof(m_ConsoleBuffer));
			}

			ImGuiListClipper clipper;
			clipper.Begin((int)m_Commands.size());

			while (clipper.Step())
			{
				for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
				{
					auto command = m_Commands[i];
					ImGui::Text(command.c_str());
				}
			}

			clipper.End();

			ImGui::End();
		}
		else
		{
			ImGui::End();
		}
	}
}
