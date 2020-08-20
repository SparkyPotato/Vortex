#pragma once
#include <Math/Matrix.h>
#include <World/Components/TransformComponent.h>

namespace Vortex
{
	constexpr float PHI = 1.618f;

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

		unsigned int GetOwnerID() { return m_Owner; }

		void SetCameraSettings(CameraProjection projection = CameraProjection::Perspective, float aspectRatio = 1.f, float nearPlane = 0.5f, float farPlane = 1000.f, float FOVMultiple = PHI);
		void SetProjection(CameraProjection projection = CameraProjection::Perspective);
		void SetNearPlane(float nearPlane = 0.5f);
		void SetFarPlane(float farPlane = 1000.f);
		void SetFOVMultiple(float FOVMultiple = PHI);

		void Resize(float aspectRatio);

		CameraProjection GetProjectionMode() { return m_Projection; }
		float GetAspectRatio() { return m_AspectRatio; }
		float GetNearPlane() { return m_NearPlane; }
		float GetFarPlane() { return m_FarPlane; }

		const Math::Matrix GetViewProjectionMatrix() { return m_Transform->GetTransformInverse() * m_ProjectionMatrix; }

		Math::Vector GetForwardVector();

	private:
		unsigned int m_Owner;
		World* m_World;

		Math::Matrix m_ProjectionMatrix;

		TransformComponent* m_Transform;

		CameraProjection m_Projection;
		float m_Multiple;
		float m_AspectRatio;
		float m_NearPlane;
		float m_FarPlane;
	};
}
