#include <VXpch.h>
#include <World/Entity.h>
#include <World/World.h>

namespace Vortex
{
	Entity::Entity(unsigned int id, std::string name, World* world)
		: m_ID(id), m_Name(name), m_World(world)
	{
		m_World->CreateTransformComponent(m_ID);
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

	Entity* Entity::AddChild(std::string name)
	{
		Entity* child = m_World->CreateEntity(name);
		child->SetParentFromID(m_ID);

		return child;
	}

	Entity* Entity::GetParent()
	{
		return m_World->GetEntityFromID(m_ParentID);
	}
}
