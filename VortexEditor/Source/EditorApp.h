#pragma once
#include <VortexCore.h>

class EditorApp : public Vortex::IApplication
{
public:
	EditorApp();
	virtual ~EditorApp();

	virtual const Vortex::IWindow::Properties& GetWindowProperties() override;

	virtual void Start() override;
	virtual void Tick(float deltaTime) override;

private:
	Vortex::IWindow::Properties m_WindowProperties;
};
