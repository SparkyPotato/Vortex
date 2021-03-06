#pragma once
#include <World/Entity.h>
#include <World/Components/TransformComponent.h>
#include <World/Components/MeshComponent.h>
#include <World/Components/SpriteComponent.h>
#include <World/Components/CameraComponent.h>
#include <World/Components/GridComponent.h>

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

		struct WorldNode
		{
			WorldNode(Entity* node)
			{
				entity = node;
			}

			bool operator==(Entity* node)
			{
				return entity == node;
			}

			Entity* entity;
			std::vector<WorldNode> children;
		};

		Entity* CreateEntity(std::string name);
		Entity* CreateNonHierarchicalEntity(std::string name);
		Entity* CreateChildEntity(Entity* parent, std::string name);

		void DestroyEntity(unsigned int entityID);
		void DestroyEntity(Entity* entity);

		Entity* GetEntityFromID(unsigned int id);

		std::vector<Entity*>& GetEntities() { return m_Entities; }
		std::vector<TransformComponent*>& GetTransforms() { return m_Transforms; }
		std::vector<MeshComponent*>& GetMeshes() { return m_Meshes; }
		std::vector<SpriteComponent*>& GetSprites() { return m_Sprites; }
		std::vector<CameraComponent*>& GetCameras() { return m_Cameras; }

		CameraComponent* GetMainCamera() { return m_MainCamera; }
		void SetMainCamera(CameraComponent* camera) { m_MainCamera = camera; }

		const WorldNode& GetHierarchy() { return m_RootNode; }

		void SetEntityParent(Entity* entity, Entity* parent);

		TransformComponent* CreateTransformComponent(unsigned int entityID);
		void DeleteTransformComponent(unsigned int entityID);

		MeshComponent* CreateMeshComponent(unsigned int entityID);
		void DeleteMeshComponent(unsigned int entityID);

		SpriteComponent* CreateSpriteComponent(unsigned int entityID, float width = 10.f, float height = 10.f, Math::Matrix colors = { 1.f, 1.f, 1.f, 1.f });
		void DeleteSpriteComponent(unsigned int entityID);

		CameraComponent* CreateCameraComponent(unsigned int entityID);
		void DeleteCameraComponent(unsigned int entityID);

		void AddGrid(float sideLength, float largeLineDistance, float smallLineDistance, const Math::Vector& color);
		Grid* GetGrid() { return m_Grid; }

	private:
		WorldNode* FindEntityNodeParent(WorldNode* root, Entity* entity);
		WorldNode* FindEntityNode(WorldNode* root, Entity* entity);
		void DeleteComponents(unsigned int owner);

		unsigned int m_NextID = 0;

		WorldNode m_RootNode;

		std::vector<Entity*> m_Entities;
		std::vector<TransformComponent*> m_Transforms;
		std::vector<MeshComponent*> m_Meshes;
		std::vector<SpriteComponent*> m_Sprites;
		std::vector<CameraComponent*> m_Cameras;

		Grid* m_Grid = nullptr;

		CameraComponent* m_MainCamera =  nullptr;
	};
}
