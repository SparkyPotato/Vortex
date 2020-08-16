#include <VXpch.h>
#include <World/World.h>

namespace Vortex
{
	World::World()
	{
		m_Entities.reserve(100);
		m_Transforms.reserve(100);
	}

	World::~World()
	{

	}

	Entity* World::CreateEntity(std::string name)
	{
		return &m_Entities.emplace_back(m_NextID++, name, this);
	}

	void World::DestroyEntity(unsigned int entityID)
	{
		auto it = std::find(m_Entities.begin(), m_Entities.end(), entityID);

		if (it != m_Entities.end())
		{
			m_Entities.erase(it);
		}
	}

	void World::DestroyEntity(Entity* entity)
	{
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

	TransformComponent* World::CreateTransformComponent(unsigned int entityID)
	{
		return &m_Transforms.emplace_back(entityID, this);
	}

	MeshComponent* World::CreateMeshComponent(unsigned int entityID)
	{
		return &m_Meshes.emplace_back(entityID, this);
	}

	SpriteComponent* World::CreateSpriteComponent(unsigned int entityID, float width, float height, Math::Matrix colors)
	{
		return &m_Sprites.emplace_back(entityID, this, width, height, colors);
	}

	CameraComponent* World::CreateCameraComponent(unsigned int entityID)
	{
		return &m_Cameras.emplace_back(entityID, this);
	}
}
