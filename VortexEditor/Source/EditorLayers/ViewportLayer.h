#pragma once
#include <VortexMinimal.h>


class ViewportLayer : public Vortex::ILayer 
{
public:
	ViewportLayer();
	~ViewportLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void Tick(float deltaTime) override;
	virtual void OnGuiRender() override;

private:
	
};
