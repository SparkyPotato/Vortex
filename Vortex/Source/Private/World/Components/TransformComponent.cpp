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
		auto entity = m_World->GetEntityFromID(m_Owner);

		auto mesh = entity->GetMeshComponent();
		if (mesh)
			mesh->CalculateMatrix(m_Position, m_Rotation, m_Scale);

		auto sprite = entity->GetSpriteComponent();
		if (sprite)
			sprite->GetQuad().CalculateMatrix(m_Position, m_Rotation, m_Scale);

		auto camera = entity->GetCameraComponent();
		if (camera)
			camera->CalculateMatrix(m_Position, m_Rotation, m_Scale);
	}

}
