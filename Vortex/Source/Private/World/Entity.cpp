#include <VXpch.h>
#include <World/Entity.h>
#include <World/World.h>

namespace Vortex
{
	Entity::Entity(unsigned int id, std::string name)
		: m_ID(id), m_Name(name)
	{

	}

	Entity::~Entity()
	{

	}

	bool Entity::operator==(const Entity& other)
	{
		return m_ID == other.m_ID;
	}

	bool Entity::operator==(unsigned int id)
	{
		return m_ID == id;
	}

	Entity* Entity::GetParent()
	{
		return m_World->GetEntityFromID(m_ParentID);
	}
}
