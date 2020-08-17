#pragma once
#include <Math/Matrix.h>
#include <World/Components/TransformComponent.h>

namespace Vortex
{
	class World;

	enum class CameraProjection
	{
		Perspective,
		Orthographic
	};

	class CameraComponent
	{
	public:
		CameraComponent(unsigned int owner, World* world);
		~CameraComponent();

		CameraComponent& operator=(const CameraComponent& other)
		{
			m_Owner = other.m_Owner;
			m_World = other.m_World;
			m_ProjectionMatrix = other.m_ProjectionMatrix;
			m_Transform = other.m_Transform;
			m_AspectRatio = other.m_AspectRatio;
			m_NearPlane = other.m_NearPlane;
			m_FarPlane = other.m_FarPlane;

			return *this;
		}

		void SetCameraSettings(CameraProjection projection = CameraProjection::Perspective, float aspectRatio = 1.f, float nearPlane = 0.1f, float farPlane = 1000.f);
		void Resize(float aspectRatio);

		CameraProjection GetProjectionMode() { return m_Projection; }
		float GetAspectRatio() { return m_AspectRatio; }
		float GetNearPlane() { return m_NearPlane; }
		float GetFarPlane() { return m_FarPlane; }

		unsigned int GetOwnerID() { return m_Owner; }

		const Math::Matrix GetViewProjectionMatrix() { return m_Transform.GetTransformInverse() * m_ProjectionMatrix; }

		Math::Vector GetForwardVector();

	private:
		unsigned int m_Owner;
		World* m_World;

		Math::Matrix m_ProjectionMatrix;

		TransformComponent& m_Transform;

		CameraProjection m_Projection;
		float m_AspectRatio;
		float m_NearPlane;
		float m_FarPlane;
	};
}
