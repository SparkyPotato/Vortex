#include <VXpch.h>
#include <World/Components/TransformComponent.h>
#include <World/World.h>

namespace Vortex
{
	TransformComponent::TransformComponent(unsigned int owner, World* world)
		: m_Scale(1.f), m_Owner(owner), m_World(world)
	{

	}

	TransformComponent::~TransformComponent()
	{

	}

	void TransformComponent::SetPosition(const Math::Vector& position)
	{
		m_Position = position;
		MoveComponents();
	}

	void TransformComponent::SetRotation(const Math::Vector& rotation)
	{
		m_Rotation = rotation;
		MoveComponents();
	}

	void TransformComponent::SetScale(const Math::Vector& scale)
	{
		m_Scale = scale;
		MoveComponents();
	}

	void TransformComponent::MoveComponents()
	{
		auto meshes = m_World->GetMeshes();
		for (auto& mesh : meshes)
		{
			if (mesh.GetOwnerID() == m_Owner)
			{
				mesh.CalculateMatrix(m_Position, m_Rotation, m_Scale);
				break;
			}
		}

		auto sprites = m_World->GetSprites();
		for (auto& sprite : sprites)
		{
			if (sprite.GetOwnerID() == m_Owner)
			{
				sprite.GetQuad().CalculateMatrix(m_Position, m_Position, m_Scale);
				break;
			}
		}
	}
}
