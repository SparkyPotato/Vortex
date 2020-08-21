#pragma once
#include <VortexMinimal.h>

class ProfilerLayer : public Vortex::Layer
{
public:
	ProfilerLayer(bool* isProfilerOpen);
	virtual ~ProfilerLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void Tick(float deltaTime) override;
	virtual void OnGuiRender() override;

private:
	bool* m_IsOpen;
	bool m_IsPaused = false;

	float m_CurrentDeltaTime;
	float m_PausedDeltaTime;

	float m_TickDeltaTime;
	float m_PausedTickDeltaTime;

	std::map<std::string, float> m_PausedProfiles;
};
