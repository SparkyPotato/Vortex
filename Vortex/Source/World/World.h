#pragma once
#include <World/Entity.h>
#include <World/Components/TransformComponent.h>
#include <World/Components/MeshComponent.h>
#include <World/Components/SpriteComponent.h>
#include <World/Components/CameraComponent.h>

namespace Vortex
{
	class TransformComponent;
	class MeshComponent;
	class SpriteComponent;

	class World
	{
	public:
		World();
		~World();

		Entity* CreateEntity(std::string name);

		void DestroyEntity(unsigned int entityID);
		void DestroyEntity(Entity* entity);

		Entity* GetEntityFromID(unsigned int id);

		std::vector<Entity>& GetEntities() { return m_Entities; }
		std::vector<TransformComponent>& GetTransforms() { return m_Transforms; }
		std::vector<MeshComponent>& GetMeshes() { return m_Meshes; }
		std::vector<SpriteComponent>& GetSprites() { return m_Sprites; }
		std::vector<CameraComponent>& GetCameras() { return m_Cameras; }

		TransformComponent* CreateTransformComponent(unsigned int entityID);
		MeshComponent* CreateMeshComponent(unsigned int entityID);
		SpriteComponent* CreateSpriteComponent(unsigned int entityID);
		CameraComponent* CreateCameraComponent(unsigned int entityID);

	private:
		unsigned int m_NextID = 1;

		std::vector<Entity> m_Entities;
		std::vector<TransformComponent> m_Transforms;
		std::vector<MeshComponent> m_Meshes;
		std::vector<SpriteComponent> m_Sprites;
		std::vector<CameraComponent> m_Cameras;
	};
}
