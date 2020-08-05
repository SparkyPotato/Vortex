#include <VXpch.h>
#include <World/Component.h>
#include <World/World.h>

namespace Vortex
{
	Component::Component()
	{

	}

	Component::~Component()
	{

	}

	Entity* Component::GetOwner()
	{
		return m_World->GetEntityFromID(m_Owner);
	}
}
