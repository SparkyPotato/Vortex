#pragma once
#include <Math/Vector.h>

namespace Vortex
{
	class TransformComponent
	{
	public:
		TransformComponent(unsigned int owner);
		~TransformComponent();

		unsigned int GetOwnerID() { return m_Owner; }

		const Math::Vector& GetPosition() { return m_Position; }
		const Math::Vector& GetRotation() { return m_Rotation; }
		const Math::Vector& GetScale() { return m_Scale; }

		void SetPosition(const Math::Vector& position) { m_Position = position; }
		void SetRotation(const Math::Vector& rotation) { m_Rotation = rotation; }
		void SetScale(const Math::Vector& scale) { m_Scale = scale; }

	private:
		unsigned int m_Owner;

		Math::Vector m_Position;
		Math::Vector m_Rotation;
		Math::Vector m_Scale;
	};
}
