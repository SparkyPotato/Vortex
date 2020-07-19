#pragma once
#include <VortexCore.h>

class EditorApp : public Vortex::IApplication
{
public:
	EditorApp();
	virtual ~EditorApp();

	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
};

Vortex::IApplication* CreateApplication()
{
	return new EditorApp();
}
