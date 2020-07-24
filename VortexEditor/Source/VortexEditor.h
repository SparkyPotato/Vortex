#pragma once
#include <VortexMinimal.h>

class VortexEditor : public Vortex::IApplication
{
public:
	VortexEditor();
	virtual ~VortexEditor();

	virtual Vortex::IWindow::Properties GetWindowProperties() override 
	{ return Vortex::IWindow::Properties("Vortex Editor", 1600, 900, 1280, 720); }

	virtual void Start() override;
	virtual void Tick(float deltaTime) override;

private:
};
