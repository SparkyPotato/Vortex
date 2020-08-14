#pragma once
#include <World/Entity.h>
#include <World/Components/TransformComponent.h>
#include <World/Components/MeshComponent.h>
#include <World/Components/SpriteComponent.h>

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

		const std::vector<Entity>& GetEntities() { return m_Entities; }
		const std::vector<TransformComponent>& GetTransforms() { return m_Transforms; }
		const std::vector<MeshComponent>& GetMeshes() { return m_Meshes; }
		const std::vector<SpriteComponent>& GetSprites() { return m_Sprites; }

		TransformComponent* CreateTransformComponent(unsigned int entityID);
		MeshComponent* CreateMeshComponent(unsigned int entityID);
		SpriteComponent* CreateSpriteComponent(unsigned int entityID);

	private:
		unsigned int m_NextID = 1;

		std::vector<Entity> m_Entities;
		std::vector<TransformComponent> m_Transforms;
		std::vector<MeshComponent> m_Meshes;
		std::vector<SpriteComponent> m_Sprites;
	};
}
