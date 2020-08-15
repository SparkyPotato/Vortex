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

	void ShowLogText();

private:
	bool* m_IsOpen;

	int m_EngineLogLevel = 0;
	int m_ClientLogLevel = 0;
};
