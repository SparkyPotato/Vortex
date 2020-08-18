#include <VXpch.h>
#include <World/Components/CameraComponent.h>
#include <World/World.h>

constexpr float PHI = 1.618f;

namespace Vortex
{
	CameraComponent::CameraComponent(unsigned int owner, World* world)
		: m_Owner(owner), m_World(world)
	{
		m_Transform = m_World->GetEntityFromID(m_Owner)->GetTransform();
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

		if (m_Projection == CameraProjection::Orthographic)
		{
			m_ProjectionMatrix = Math::Matrix::Orthographic(m_AspectRatio * PHI, PHI, m_FarPlane, m_NearPlane);
		}
		else
		{
			m_ProjectionMatrix = Math::Matrix::Perspective(m_AspectRatio * PHI, PHI, m_FarPlane, m_NearPlane);
		}
	}

	void CameraComponent::Resize(float aspectRatio)
	{
		SetCameraSettings(m_Projection, aspectRatio, m_NearPlane, m_FarPlane);
	}

	Math::Vector CameraComponent::GetForwardVector()
	{
		auto rotation = m_Transform->GetRotationMatrix();
		return Math::Vector(0.f, 0.f, 1.f) * rotation;
	}
}
