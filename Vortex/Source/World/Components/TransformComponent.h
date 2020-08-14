#pragma once
#include <Math/Vector.h>

namespace Vortex
{
	class World;

	class TransformComponent
	{
	public:
		TransformComponent(unsigned int owner, World* world);
		~TransformComponent();

		unsigned int GetOwnerID() { return m_Owner; }
		
		const Math::Vector& GetPosition() { return m_Position; }
		const Math::Vector& GetRotation() { return m_Rotation; }
		const Math::Vector& GetScale() { return m_Scale; }

		void SetPosition(const Math::Vector& position);
		void SetRotation(const Math::Vector& rotation);
		void SetScale(const Math::Vector& scale);

	private:
		void MoveComponents();

		unsigned int m_Owner;
		World* m_World;

		Math::Vector m_Position;
		Math::Vector m_Rotation;
		Math::Vector m_Scale;
	};
}
