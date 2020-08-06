#include <VXpch.h>
#include <World/Components/TransformComponent.h>

namespace Vortex
{
	TransformComponent::TransformComponent(unsigned int owner)
		: m_Scale(1.f), m_Owner(owner)
	{

	}

	TransformComponent::~TransformComponent()
	{

	}
}
