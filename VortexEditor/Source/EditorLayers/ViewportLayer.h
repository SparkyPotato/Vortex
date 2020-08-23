#pragma once
#include <VortexMinimal.h>
#include <World/World.h>

DECLARE_LOGGER(LogViewport);

class WorldLayer;

class ViewportLayer : public Vortex::Layer 
{
public:
	ViewportLayer(bool* isViewportCurrentlyOpen, WorldLayer* worldLayer);
	virtual ~ViewportLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void Tick(float deltaTime) override;

	virtual void OnGuiRender() override;
	virtual void On3dUiRender() override;

private:
	void HandleResize(int width, int height);

	bool* m_IsOpen;

	Vortex::GPTexture* m_Texture;
	Vortex::GPFramebuffer* m_Framebuffer;

	WorldLayer* m_WorldLayer;
	Vortex::Entity* m_EditorEntity = nullptr;

	bool m_IsMouseInViewportBounds = false;
	bool m_IsViewportActive = false;
	ImVec2 m_ViewportTopLeft;
	ImVec2 m_ViewportBottomRight;
	ImVec2 m_LastFrameMousePosition;

	float m_MovementSpeed = 3.f;
	float m_Sensitivity = 1.f;

	float translation[3] = { 0.f, 0.f, 0.f };
};
