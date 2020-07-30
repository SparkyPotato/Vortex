#pragma once
#include <VortexMinimal.h>

class ProfilerLayer : public Vortex::Layer
{
public:
	ProfilerLayer(bool* isProfilerOpen);
	~ProfilerLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void Tick(float deltaTime) override;
	virtual void OnGuiRender() override;

private:
	bool* m_IsOpen;
	bool m_IsPaused = false;

	float m_CurrentDeltaTime;
	float m_PausedDeltaTime;

	std::map<std::string, float> m_PausedProfiles;
};
