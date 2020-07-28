#pragma once
#include <VortexMinimal.h>

class EditorApp : public Vortex::IApplication
{
public:
	EditorApp();
	virtual ~EditorApp();

	virtual Vortex::IWindow::Properties GetWindowProperties() override 
	{ return Vortex::IWindow::Properties("Vortex Editor", 1600, 900, 1280, 720); }

	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual void OnGuiRender() override;

	virtual void OnEvent(Vortex::IEvent& event) override;

private:
	bool m_IsShortcutPressed = false;

	bool m_IsViewportOpen = true;

	bool m_IsLogOpen = false;

	bool m_ShowStats = false;
};
