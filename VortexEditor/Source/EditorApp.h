#pragma once
#include <VortexMinimal.h>

class EditorApp : public Vortex::Application
{
public:
	EditorApp();
	virtual ~EditorApp();

	virtual Vortex::Window::Properties GetWindowProperties() override 
	{ return Vortex::Window::Properties("Vortex Editor", 1600, 900, 1280, 720); }

	virtual void Start() override;
	virtual void Quit() override;
	virtual void Tick(float deltaTime) override;
	virtual void OnGuiRender() override;

	virtual void OnEvent(Vortex::Event& event) override;

private:
	void LoadPrefs(std::string file);
	void SavePrefs(std::string file);

	bool m_IsClosing = false;

	bool m_IsViewportOpen = true;
	bool m_IsLogOpen = false;
	bool m_IsProfilerOpen = false;
};
