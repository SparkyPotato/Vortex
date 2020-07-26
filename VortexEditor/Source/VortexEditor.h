#pragma once
#include <VortexMinimal.h>
#include <Graphics/Primitives/GPFramebuffer.h>

class VortexEditor : public Vortex::IApplication
{
public:
	VortexEditor();
	virtual ~VortexEditor();

	virtual Vortex::IWindow::Properties GetWindowProperties() override 
	{ return Vortex::IWindow::Properties("Vortex Editor", 1600, 900, 1280, 720); }

	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual void OnEvent(Vortex::IEvent& event) override;

private:
	
};
