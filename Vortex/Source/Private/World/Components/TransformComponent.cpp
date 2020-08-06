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
		auto meshes = m_World->GetMeshes();

		for (auto& mesh : meshes)
		{
			if (mesh.GetOwnerID() == m_Owner)
			{
				mesh.CalculateMatrix(m_Position, m_Rotation, m_Scale);
				break;
			}
		}
	}

	void TransformComponent::SetRotation(const Math::Vector& rotation)
	{
		m_Rotation = rotation;
		auto meshes = m_World->GetMeshes();

		for (auto& mesh : meshes)
		{
			if (mesh.GetOwnerID() == m_Owner)
			{
				mesh.CalculateMatrix(m_Position, m_Rotation, m_Scale);
				break;
			}
		}
	}

	void TransformComponent::SetScale(const Math::Vector& scale)
	{
		m_Scale = scale;
		auto meshes = m_World->GetMeshes();

		for (auto& mesh : meshes)
		{
			if (mesh.GetOwnerID() == m_Owner)
			{
				mesh.CalculateMatrix(m_Position, m_Rotation, m_Scale);
				break;
			}
		}
	}
}
