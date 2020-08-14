#pragma once

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
		Entity(unsigned int id, std::string name, World* world);
		~Entity();

		bool operator==(const Entity& other);
		bool operator==(unsigned int id);

		unsigned int GetID() { return m_ID; }

		Entity* AddChild(std::string name);

		Entity* GetParent();
		unsigned int GetParentID() { return m_ParentID; }
		void SetParent(Entity* parent) { m_ParentID = parent->GetID(); }
		void SetParentFromID(unsigned int parentID) { m_ParentID = parentID; }

		std::string GetName() { return m_Name; }
		void SetName(std::string name) { m_Name = name; }

		TransformComponent& GetTransform() { return *m_Transform; }

		MeshComponent* AddMeshComponent();
		MeshComponent* GetMeshComponent();

		SpriteComponent* AddSpriteComponent();
		SpriteComponent* GetSpriteComponent();

		CameraComponent* AddCameraComponent();
		CameraComponent* GetCameraComponent();

	private:
		unsigned int m_ID;
		std::string m_Name;

		unsigned int m_ParentID = 0;

		World* m_World;

		TransformComponent* m_Transform;
	};
}
