#pragma once
#include <VortexMinimal.h>

class ConsoleLayer : public Vortex::Layer 
{
public:
	ConsoleLayer(bool* isConsoleCurrentlyOpen);
	virtual ~ConsoleLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void Tick(float deltaTime) override;
	virtual void OnGuiRender() override;

private:
	bool* m_IsOpen;

	char m_ConsoleBuffer[512];

	std::vector<std::string> m_Commands;
};
