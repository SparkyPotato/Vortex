#pragma once
#include <VortexMinimal.h>
#include <World/World.h>

DECLARE_LOGGER(LogWorld);

class WorldLayer : public Vortex::Layer
{
public:
	WorldLayer(bool* isWorldOpen, bool* isPropertiesOpen);
	virtual ~WorldLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void Tick(float deltaTime) override;
	virtual void OnGuiRender() override;

private:
	void DisplayChildren(const Vortex::World::WorldNode& node);

	void SetCurrentEntity(Vortex::Entity* entity);

	void DrawAddEntity();

	void DrawProperties();
	void DrawTransform();

	Vortex::Entity* m_CurrentlySelectedEntity = nullptr;
	float m_Position[3];
	float m_Rotation[3];
	float m_Scale[3];

	bool* m_IsWorldOpen;

	bool* m_IsPropertiesOpen;

	bool m_OpenAddEntity = false;
	bool m_OpenEntityContext = false;

	Vortex::World* m_World;

	char m_NewEntityNameBuffer[100];
	char m_EntityNameBuffer[100];
};
