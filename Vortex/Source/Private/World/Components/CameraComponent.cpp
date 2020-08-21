#include <VXpch.h>
#include <World/Components/CameraComponent.h>
#include <World/World.h>

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

	void CameraComponent::SetCameraSettings(CameraProjection projection, float aspectRatio, float nearPlane, float farPlane, float FOVMultiple)
	{
		m_Projection = projection;
		m_AspectRatio = aspectRatio;
		m_NearPlane = nearPlane;
		m_FarPlane = farPlane;
		m_Multiple = FOVMultiple;

		if (m_Projection == CameraProjection::Orthographic)
		{
			m_ProjectionMatrix = Math::Matrix::Orthographic(m_AspectRatio * m_Multiple, m_Multiple, m_FarPlane, m_NearPlane);
		}
		else
		{
			m_ProjectionMatrix = Math::Matrix::Perspective(m_AspectRatio * m_Multiple, m_Multiple, m_FarPlane, m_NearPlane);
		}
	}

	void CameraComponent::SetProjection(CameraProjection projection)
	{
		SetCameraSettings(projection, m_AspectRatio, m_NearPlane, m_FarPlane, m_Multiple);
	}

	void CameraComponent::SetNearPlane(float nearPlane)
	{
		SetCameraSettings(m_Projection, m_AspectRatio, nearPlane, m_FarPlane, m_Multiple);
	}

	void CameraComponent::SetFarPlane(float farPlane)
	{
		SetCameraSettings(m_Projection, m_AspectRatio, m_NearPlane, farPlane, m_Multiple);
	}

	void CameraComponent::SetFOVMultiple(float FOVMultiple)
	{
		SetCameraSettings(m_Projection, m_AspectRatio, m_NearPlane, m_FarPlane, FOVMultiple);
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

	Math::Vector CameraComponent::GetRightVector()
	{
		auto rotation = m_Transform->GetRotationMatrix();
		return Math::Vector(1.f, 0.f, 0.f) * rotation;
	}

	Math::Vector CameraComponent::GetUpVector()
	{
		auto rotation = m_Transform->GetRotationMatrix();
		return Math::Vector(0.f, 1.f, 0.f) * rotation;
	}

}
