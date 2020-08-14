#include <VXpch.h>
#include <World/Components/CameraComponent.h>
#include <World/World.h>

namespace Vortex
{
	CameraComponent::CameraComponent(unsigned int owner, World* world)
		: m_Owner(owner), m_World(world)
	{
		m_Transform = &m_World->GetEntityFromID(m_Owner)->GetTransform();

		SetCameraSettings();
	}

	CameraComponent::~CameraComponent()
	{

	}

	void CameraComponent::SetCameraSettings(CameraProjection projection, float aspectRatio, float nearPlane, float farPlane)
	{
		m_Projection = projection;
		m_AspectRatio = aspectRatio;
		m_NearPlane = nearPlane;
		m_FarPlane = farPlane;

		CalculateMatrix(m_Transform->GetPosition(), m_Transform->GetRotation(), m_Transform->GetScale());
	}

	void CameraComponent::Resize(float aspectRatio)
	{
		SetCameraSettings(m_Projection, aspectRatio, m_NearPlane, m_FarPlane);
	}

	void CameraComponent::CalculateMatrix(const Math::Vector& position, const Math::Vector& rotation, const Math::Vector& scale)
	{
		using namespace Math;

		Matrix viewMatrix = Matrix::Translate(-position) * Matrix::Rotate(-rotation);
		
		Matrix projectionMatrix;
		if (m_Projection == CameraProjection::Orthographic)
		{
			projectionMatrix = Matrix::Orthographic(2 * m_AspectRatio, 2.f, m_FarPlane, m_NearPlane);
		}
		else
		{
			projectionMatrix = Matrix::Perspective(2 * m_AspectRatio, 2.f, m_FarPlane, m_NearPlane);
		}

		m_ViewProjection = viewMatrix * projectionMatrix;
	}

	Math::Vector CameraComponent::GetForwardVector()
	{
		auto rotation = Math::Matrix::Rotate(m_Transform->GetRotation());
		return Math::Vector(0.f, 0.f, 1.f) * rotation;
	}
}
