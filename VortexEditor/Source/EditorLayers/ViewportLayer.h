#pragma once
#include <VortexMinimal.h>
#include <Graphics/Primitives/GPVertexBuffer.h>
#include <Graphics/Primitives/GPIndexBuffer.h>
#include <Graphics/Primitives/GPVertexShader.h>
#include <Graphics/Primitives/GPPixelShader.h>

class ViewportLayer : public Vortex::Layer 
{
public:
	ViewportLayer(bool* isViewportCurrentlyOpen);
	~ViewportLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void Tick(float deltaTime) override;
	virtual void OnGuiRender() override;

private:
	void HandleResize(int width, int height);

	bool* m_IsOpen;

	Vortex::GPTexture* m_Texture;
	Vortex::GPFramebuffer* m_Framebuffer;

	Vortex::World* m_World;
};
