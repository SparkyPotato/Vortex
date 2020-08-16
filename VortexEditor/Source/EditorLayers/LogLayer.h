#pragma once
#include <VortexMinimal.h>

class LogLayer : public Vortex::Layer
{
public:
	LogLayer(bool* isLogCurrentlyOpen);
	virtual ~LogLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void Tick(float deltaTime) override;
	virtual void OnGuiRender() override;

	void OpenConsole();

	void ShowLogText();
	void UpdateLog();

private:
	bool* m_IsOpen;

	std::vector<Vortex::Log> m_LogsToShow;
	int m_LogLevel = 0;

	bool m_IsConsoleOpen = false;
	bool m_WasJustOpened = false;
	char m_ConsoleBuffer[1024];
};
