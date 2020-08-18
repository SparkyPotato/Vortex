#include <VXpch.h>
#include <World/Components/MeshComponent.h>
#include <World/World.h>

namespace Vortex
{
	MeshComponent::MeshComponent(unsigned int owner, World* world)
		: m_Owner(owner), m_World(world)
	{
		m_Transform = m_World->GetEntityFromID(m_Owner)->GetTransform();
	}

	MeshComponent::~MeshComponent()
	{
		if (m_Vertices) delete m_Vertices;
		if (m_Indices) delete m_Indices;
	}

	void MeshComponent::LoadFromFile(std::string file)
	{
		if (m_Vertices) delete m_Vertices;
		if (m_Indices) delete m_Indices;


	}
}
