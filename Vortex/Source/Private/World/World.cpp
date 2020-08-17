#include <VXpch.h>
#include <World/World.h>

namespace Vortex
{
	World::World()
		: m_RootNode(new Entity(m_NextID++, "Root", this))
	{
		m_Entities.reserve(100);
		m_Transforms.reserve(100);

		m_RootNode.entity->m_ParentID = -1;
	}

	World::~World()
	{

	}

	Entity* World::CreateEntity(std::string name)
	{
		Entity* entity = &m_Entities.emplace_back(m_NextID++, name, this);
		entity->m_Transform = CreateTransformComponent(entity->m_ID);

		m_RootNode.children.emplace_back(entity);

		return entity;
	}

	Entity* World::CreateChildEntity(Entity* parent, std::string name)
	{
		Entity* entity = &m_Entities.emplace_back(m_NextID++, name, this);
		entity->m_Transform = CreateTransformComponent(entity->m_ID);

		auto parentNode = FindEntityNode(&m_RootNode, parent);
		parentNode->children.emplace_back(entity);

		return entity;
	}

	void World::DestroyEntity(unsigned int entityID)
	{
		auto it = std::find(m_Entities.begin(), m_Entities.end(), entityID);

		DeleteComponents(entityID);

		WorldNode* oldNode;
		if (oldNode = FindEntityNodeParent(&m_RootNode, &(*it)))
		{
			auto newit = std::find(oldNode->children.begin(), oldNode->children.end(), &(*it));
			oldNode->children.erase(newit);
		}
		if (oldNode = FindEntityNode(&m_RootNode, &(*it)))
		{
			for (auto& child : oldNode->children)
			{
				DestroyEntity(child.entity);
			}
		}

		it = std::find(m_Entities.begin(), m_Entities.end(), entityID);

		if (it != m_Entities.end())
		{
			m_Entities.erase(it);
		}
	}

	void World::DestroyEntity(Entity* entity)
	{
		DeleteComponents(entity->GetID());

		WorldNode* oldNode;
		if (oldNode = FindEntityNodeParent(&m_RootNode, entity))
		{
			auto it = std::find(oldNode->children.begin(), oldNode->children.end(), entity);
			oldNode->children.erase(it);
		}
		if (oldNode = FindEntityNode(&m_RootNode, entity))
		{
			for (auto& child : oldNode->children)
			{
				DestroyEntity(child.entity);
			}
		}

		auto it = std::find(m_Entities.begin(), m_Entities.end(), *entity);

		if (it != m_Entities.end())
		{
			m_Entities.erase(it);
		}
	}

	Entity* World::GetEntityFromID(unsigned int id)
	{
		auto it = std::find(m_Entities.begin(), m_Entities.end(), id);

		if (it != m_Entities.end())
		{
			return &(*it);
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
		return &m_Transforms.emplace_back(entityID, this);
	}

	void World::DeleteTransformComponent(unsigned int entityID)
	{
		for (auto it = m_Transforms.begin(); it < m_Transforms.end(); it++)
		{
			if (it->GetOwnerID() == entityID)
			{
				m_Transforms.erase(it);
				break;
			}
		}
	}

	MeshComponent* World::CreateMeshComponent(unsigned int entityID)
	{
		return &m_Meshes.emplace_back(entityID, this);
	}

	void World::DeleteMeshComponent(unsigned int entityID)
	{
		for (auto it = m_Meshes.begin(); it < m_Meshes.end(); it++)
		{
			if (it->GetOwnerID() == entityID)
			{
				m_Meshes.erase(it);
				break;
			}
		}
	}

	SpriteComponent* World::CreateSpriteComponent(unsigned int entityID, float width, float height, Math::Matrix colors)
	{
		return &m_Sprites.emplace_back(entityID, this, width, height, colors);
	}

	void World::DeleteSpriteComponent(unsigned int entityID)
	{
		for (auto it = m_Sprites.begin(); it < m_Sprites.end(); it++)
		{
			if (it->GetOwnerID() == entityID)
			{
				m_Sprites.erase(it);
				break;
			}
		}
	}

	CameraComponent* World::CreateCameraComponent(unsigned int entityID)
	{
		return &m_Cameras.emplace_back(entityID, this);
	}

	void World::DeleteCameraComponent(unsigned int entityID)
	{
		for (auto it = m_Cameras.begin(); it < m_Cameras.end(); it++)
		{
			if (it->GetOwnerID() == entityID)
			{
				if (&(*it) == m_MainCamera)
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
