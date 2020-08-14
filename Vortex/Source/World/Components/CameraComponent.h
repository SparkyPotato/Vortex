#pragma once
#include <Math/Matrix.h>

namespace Vortex
{
	class World;
	class TransformComponent;

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

		void SetCameraSettings(CameraProjection projection = CameraProjection::Perspective, float aspectRatio = 1.f, float nearPlane = 0.1f, float farPlane = 1000.f);
		void Resize(float aspectRatio);

		unsigned int GetOwnerID() { return m_Owner; }

		void CalculateMatrix(const Math::Vector& position, const Math::Vector& rotation, const Math::Vector& scale);

		const Math::Matrix& GetViewProjectionMatrix() { return m_ViewProjection; }

		Math::Vector GetForwardVector();

	private:
		unsigned int m_Owner;
		World* m_World;

		Math::Matrix m_ViewProjection;

		TransformComponent* m_Transform;

		CameraProjection m_Projection;
		float m_AspectRatio;
		float m_NearPlane;
		float m_FarPlane;
	};
}
