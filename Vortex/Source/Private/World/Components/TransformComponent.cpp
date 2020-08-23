#include <VXpch.h>
#include <World/Components/TransformComponent.h>
#include <World/World.h>

namespace Vortex
{
	TransformComponent::TransformComponent(unsigned int owner, World* world)
		: m_Scale(1.f), m_Owner(owner), m_World(world)
	{
		Entity* parent = m_World->GetEntityFromID(m_Owner)->GetParent();
		if (parent)
		{
			m_ParentTransform = parent->GetTransform();
		}
	}

	TransformComponent::~TransformComponent()
	{

	}

	Math::Vector TransformComponent::GetWorldPosition()
	{
		if (m_ParentTransform)
		{
			return m_ParentTransform->GetWorldPosition() + m_Position;
		}
		return m_Position;
	}

	const Math::Matrix TransformComponent::GetTransform()
	{
		if (m_ParentTransform)
		{
			return m_ParentTransform->GetTransform() * m_Transformation;
		}
		return m_Transformation;
	}

	const Math::Matrix TransformComponent::GetTransformInverse()
	{
		if (m_ParentTransform)
		{
			return m_InverseTransformation * m_ParentTransform->GetTransformInverse();
		}
		return m_InverseTransformation;
	}

	const Math::Matrix TransformComponent::GetRotationMatrix()
	{
		if (m_ParentTransform)
		{
			return m_ParentTransform->GetRotationMatrix() * m_RotationMatrix;
		}
		return m_RotationMatrix;
	}

	void TransformComponent::SetPosition(const Math::Vector& position)
	{
		m_Position = position;
		RecalculateMatrices();
	}

	void TransformComponent::SetRotation(const Math::Vector& rotation)
	{
		m_Rotation = rotation;
		RecalculateMatrices();
	}

	void TransformComponent::SetScale(const Math::Vector& scale)
	{
		m_Scale = scale;
		RecalculateMatrices();
	}

	void TransformComponent::RecalculateMatrices()
	{
		using namespace Math;

		m_Transformation = Matrix::Scale(m_Scale) * Matrix::Rotate(m_Rotation) * Matrix::Translate(m_Position);

		m_InverseTransformation = Matrix::Inverse(m_Transformation);

		m_RotationMatrix = Matrix::Rotate(m_Rotation);
	}

}
