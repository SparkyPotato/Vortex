#include <VXpch.h>
#include <World/Components/MeshComponent.h>
#include <World/World.h>

namespace Vortex
{
	MeshComponent::MeshComponent(unsigned int owner, World* world)
		: m_Owner(owner), m_World(world)
	{
		auto transform = m_World->GetEntityFromID(m_Owner)->GetTransform();
		if (!transform)
		{
			ENG_ERROR("Entity does not exist!");
			return;
		}

		CalculateMatrix(transform->GetPosition(), transform->GetRotation(), transform->GetScale());
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

	void MeshComponent::CalculateMatrix(const Math::Vector& position, const Math::Vector& rotation, const Math::Vector& scale)
	{
		using namespace Math;

		m_Transformation = Matrix::Rotate(rotation) * Matrix::Scale(scale) * Matrix::Translate(position);
	}
}
