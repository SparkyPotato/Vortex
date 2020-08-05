#pragma once
#include <World/Entity.h>

namespace Vortex
{
	class World
	{
	public:
		World();
		~World();

		Entity* CreateEntity(std::string name);

		void DestroyEntity(unsigned int entityID);
		void DestroyEntity(Entity* entity);

		Entity* GetEntityFromID(unsigned int id);

	private:
		unsigned int m_NextID = 1;

		std::vector<Entity> m_Entities;
	};
}
