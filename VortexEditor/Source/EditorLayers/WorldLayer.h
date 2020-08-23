#pragma once
#include <VortexMinimal.h>
#include <World/World.h>

DECLARE_LOGGER(LogWorld);

class WorldLayer : public Vortex::Layer
{
public:
	enum class GizmoMode
	{
		Translation,
		Rotation,
		Scale
	};

	WorldLayer(bool* isWorldOpen, bool* isPropertiesOpen);
	virtual ~WorldLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void Tick(float deltaTime) override;

	virtual void OnGuiRender() override;
	virtual void On3dUiRender() override;

	Vortex::Entity* GetEditorEntity() { return m_EditorEntity; }

	void SetGizmoMode(GizmoMode mode) { m_GizmoMode = mode; }

private:
	void FocusEntity();

	void DisplayChildren(const Vortex::World::WorldNode& node);

	void SetCurrentEntity(Vortex::Entity* entity);

	void DrawAddEntity();
	void DrawAddComponent();

	void DrawProperties();
	void DrawTransform();
	void DrawSprite();
	void DrawCamera();

	Vortex::Entity* m_EditorEntity = nullptr;

	GizmoMode m_GizmoMode = GizmoMode::Translation;

	Vortex::Entity* m_CurrentlySelectedEntity = nullptr;
	float m_Position[3];
	float m_Rotation[3];
	float m_Scale[3];

	float m_QuadSize[2];
	float m_VertexCol1[3];
	float m_VertexCol2[3];
	float m_VertexCol3[3];
	float m_VertexCol4[3];
	std::string m_CurrentTexture = "None";

	int m_Projection;
	float m_AspectRatio;
	float m_NearPlane;
	float m_FarPlane;
	float m_FOVMultiple;

	bool* m_IsWorldOpen;
	bool m_IsWorldActive = false;

	bool* m_IsPropertiesOpen;

	bool m_OpenAddEntity = false;
	bool m_OpenAddComponent = false;

	Vortex::World* m_World;

	char m_NewEntityNameBuffer[100];
	char m_EntityNameBuffer[100];

	Im3d::Mat4 m_GizmoMatrix;
};
