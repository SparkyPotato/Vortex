#pragma once
#include <VortexMinimal.h>
#include <World/World.h>

class ViewportLayer : public Vortex::Layer 
{
public:
	ViewportLayer(bool* isViewportCurrentlyOpen);
	virtual ~ViewportLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void Tick(float deltaTime) override;
	virtual void OnGuiRender() override;

private:
	void HandleResize(int width, int height);

	void MoveForward();
	void MoveBackward();
	void MoveRight();
	void MoveLeft();

	bool* m_IsOpen;

	Vortex::GPTexture* m_Texture;
	Vortex::GPFramebuffer* m_Framebuffer;

	Vortex::World* m_World;

	Vortex::Entity* m_EditorEntity;
	Vortex::CameraComponent* m_EditorCamera;
};
