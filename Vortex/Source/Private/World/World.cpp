#include <VXpch.h>
#include <World/World.h>

namespace Vortex
{
	World::World()
	{
		m_Entities.reserve(100);
	}

	World::~World()
	{

	}

	Entity* World::CreateEntity(std::string name)
	{
		auto entity = m_Entities.emplace_back(m_NextID++, name);
		entity.SetWorld(this);

		return &entity;
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
	}
}
