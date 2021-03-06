#pragma once
#include <Math/Matrix.h>

namespace Vortex
{
	class World;
	class TransformComponent;
	class MeshComponent;
	class SpriteComponent;
	class CameraComponent;

	class Entity
	{
	public:
		friend class World;

		Entity(unsigned int id, std::string name, World* world);
		~Entity();

		bool operator==(const Entity& other);
		bool operator==(unsigned int id);

		unsigned int GetID() { return m_ID; }

		Entity* AddChild(std::string name);

		Entity* GetParent();
		unsigned int GetParentID() { return m_ParentID; }
		void SetParent(Entity* parent);
		void SetParentFromID(unsigned int parentID);

		std::string GetName() { return m_Name; }
		void SetName(std::string name) { m_Name = name; }

		TransformComponent* GetTransform() { return m_Transform; }

		MeshComponent* AddMeshComponent();
		MeshComponent* GetMeshComponent();

		SpriteComponent* AddSpriteComponent(float width = 10.f, float height = 10.f, Math::Matrix colors = { 1.f, 1.f, 1.f, 1.f });
		SpriteComponent* GetSpriteComponent();

		CameraComponent* AddCameraComponent();
		CameraComponent* GetCameraComponent();

	private:
		unsigned int m_ID;
		std::string m_Name;

		unsigned int m_ParentID = 0;

		World* m_World;

		TransformComponent* m_Transform = nullptr;
		MeshComponent* m_Mesh = nullptr;
		SpriteComponent* m_Sprite = nullptr;
		CameraComponent* m_Camera = nullptr;
	};
}
