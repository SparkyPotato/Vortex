#pragma once
#include <Graphics/Primitives/GPVertexBuffer.h>
#include <Graphics/Primitives/GPIndexBuffer.h>
#include <Math/Matrix.h>

namespace Vortex
{
	class World;

	struct Quad
	{
		Quad(float height = 1.f, float width = 1.f);
		~Quad();

		void CalculateMatrix(const Math::Vector& position, const Math::Vector& rotation, const Math::Vector& scale);

		GPVertexBuffer* vertices = nullptr;
		GPIndexBuffer* indices = nullptr;

		Math::Matrix transformation;
	};

	class SpriteComponent
	{
	public:
		SpriteComponent(unsigned int owner, World* world);
		~SpriteComponent();

		unsigned int GetOwnerID() { return m_Owner; }

		Quad& GetQuad() { return m_Quad; }

	private:
		unsigned int m_Owner;
		World* m_World;

		Quad m_Quad;
	};
}
