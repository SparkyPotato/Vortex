#include <VXpch.h>
#include <World/World.h>
#include <Core/Modules/VXRenderer.h>

namespace Vortex
{
	World::World()
		: m_RootNode(new Entity(m_NextID++, "Root", this))
	{
		m_RootNode.entity->m_ParentID = -1;
	}

	World::~World()
	{
		for (auto entity : m_Entities)
		{
			delete entity;
		}
		for (auto transform : m_Transforms)
		{
			delete transform;
		}
		for (auto mesh : m_Meshes)
		{
			delete mesh;
		}
		for (auto sprite : m_Sprites)
		{
			delete sprite;
		}
		for (auto camera : m_Cameras)
		{
			delete camera;
		}
	}

	Entity* World::CreateEntity(std::string name)
	{
		if (name.empty())
			name = "Entity " + std::to_string(m_NextID);

		auto entity = new Entity(m_NextID++, name, this);
		m_Entities.push_back(entity);
		entity->m_Transform = CreateTransformComponent(entity->m_ID);

		m_RootNode.children.emplace_back(entity);

		return entity;
	}

	Entity* World::CreateChildEntity(Entity* parent, std::string name)
	{
		auto entity = new Entity(m_NextID++, name, this);
		m_Entities.push_back(entity);
		entity->m_Transform = CreateTransformComponent(entity->m_ID);

		auto parentNode = FindEntityNode(&m_RootNode, parent);
		parentNode->children.emplace_back(entity);

		return entity;
	}

	void World::DestroyEntity(unsigned int entityID)
	{
		Entity* entity = GetEntityFromID(entityID);

		DeleteComponents(entityID);

		WorldNode* oldNode;
		if (oldNode = FindEntityNode(&m_RootNode, entity))
		{
			for (auto& child : oldNode->children)
			{
				DestroyEntity(child.entity);
			}
		}

		if (oldNode = FindEntityNodeParent(&m_RootNode, entity))
		{
			auto it = std::find(oldNode->children.begin(), oldNode->children.end(), entity);
			oldNode->children.erase(it);
		}

		auto it = std::find(m_Entities.begin(), m_Entities.end(), entity);

		if (it != m_Entities.end())
		{
			delete* it;
			m_Entities.erase(it);
		}
	}

	void World::DestroyEntity(Entity* entity)
	{
		DeleteComponents(entity->GetID());

		WorldNode* oldNode;
		if (oldNode = FindEntityNode(&m_RootNode, entity))
		{
			for (auto& child : oldNode->children)
			{
				DestroyEntity(child.entity);
			}
		}

		if (oldNode = FindEntityNodeParent(&m_RootNode, entity))
		{
			auto it = std::find(oldNode->children.begin(), oldNode->children.end(), entity);
			oldNode->children.erase(it);
		}

		auto it = std::find(m_Entities.begin(), m_Entities.end(), entity);

		if (it != m_Entities.end())
		{
			delete* it;
			m_Entities.erase(it);
		}
	}

	Entity* World::GetEntityFromID(unsigned int id)
	{
		for (auto entity : m_Entities)
		{
			if (entity->GetID() == id)
			{
				return entity;
			}
		}

		return nullptr;
	}

	void World::SetEntityParent(Entity* entity, Entity* parent)
	{
		WorldNode* oldNode;
		if (oldNode = FindEntityNodeParent(&m_RootNode, entity))
		{
			auto it = std::find(oldNode->children.begin(), oldNode->children.end(), entity);
			oldNode->children.erase(it);
		}

		WorldNode* newNode;
		if (newNode = FindEntityNode(&m_RootNode, parent))
		{
			newNode->children.emplace_back(entity);
		}
	}

	TransformComponent* World::CreateTransformComponent(unsigned int entityID)
	{
		auto transform = new TransformComponent(entityID, this);
		m_Transforms.push_back(transform);

		return transform;
	}

	void World::DeleteTransformComponent(unsigned int entityID)
	{
		for (auto it = m_Transforms.begin(); it < m_Transforms.end(); it++)
		{
			if ((*it)->GetOwnerID() == entityID)
			{
				delete* it;
				m_Transforms.erase(it);
				break;
			}
		}
	}

	MeshComponent* World::CreateMeshComponent(unsigned int entityID)
	{
		auto mesh = new MeshComponent(entityID, this);
		m_Meshes.push_back(mesh);

		return mesh;
	}

	void World::DeleteMeshComponent(unsigned int entityID)
	{
		for (auto it = m_Meshes.begin(); it < m_Meshes.end(); it++)
		{
			if ((*it)->GetOwnerID() == entityID)
			{
				delete* it;
				m_Meshes.erase(it);
				break;
			}
		}
	}

	SpriteComponent* World::CreateSpriteComponent(unsigned int entityID, float width, float height, Math::Matrix colors)
	{
		auto sprite = new SpriteComponent(entityID, this, width, height, colors);
		m_Sprites.push_back(sprite);

		return sprite;
	}

	void World::DeleteSpriteComponent(unsigned int entityID)
	{
		for (auto it = m_Sprites.begin(); it < m_Sprites.end(); it++)
		{
			if ((*it)->GetOwnerID() == entityID)
			{
				delete* it;
				m_Sprites.erase(it);
				break;
			}
		}
	}

	CameraComponent* World::CreateCameraComponent(unsigned int entityID)
	{
		auto camera = new CameraComponent(entityID, this);
		m_Cameras.push_back(camera);

		return camera;
	}

	void World::DeleteCameraComponent(unsigned int entityID)
	{
		for (auto it = m_Cameras.begin(); it < m_Cameras.end(); it++)
		{
			if ((*it)->GetOwnerID() == entityID)
			{
				if (*it == m_MainCamera)
					m_MainCamera = nullptr;
				m_Cameras.erase(it);
				break;
			}
		}
	}

	World::WorldNode* World::FindEntityNodeParent(WorldNode* root, Entity* entity)
	{
		for (auto& child : root->children)
		{
			if (child.entity == entity)
				return root;
			else if (auto node = FindEntityNodeParent(&child, entity))
				return node;
		}

		return nullptr;
	}

	World::WorldNode* World::FindEntityNode(WorldNode* root, Entity* entity)
	{
		for (auto& child : root->children)
		{
			if (child.entity == entity)
				return &child;
			else if (auto node = FindEntityNode(&child, entity))
				return node;
		}

		return nullptr;
	}

	void World::DeleteComponents(unsigned int owner)
	{
		DeleteTransformComponent(owner);
		DeleteMeshComponent(owner);
		DeleteSpriteComponent(owner);
		DeleteCameraComponent(owner);
	}
}
