#include <VXpch.h>
#include <World/Components/TransformComponent.h>

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

		m_Transformation = Matrix::Rotate(m_Rotation) * Matrix::Scale(m_Scale) * Matrix::Translate(m_Position);

		m_InverseTransformation = Matrix::Translate(-m_Position) 
			* Matrix::Rotate(-m_Rotation) 
			* Matrix::Scale({ 1.f / m_Scale.x, 1.f / m_Scale.y, 1.f / m_Scale.z });

		m_RotationMatrix = Matrix::Rotate(m_Rotation);
	}

}
