#pragma once
#include <VortexMinimal.h>

class VortexEditor : public Vortex::IApplication
{
public:
	VortexEditor();
	virtual ~VortexEditor();

	virtual void Start() override;
	virtual void Tick(float deltaTime) override;

private:
};
