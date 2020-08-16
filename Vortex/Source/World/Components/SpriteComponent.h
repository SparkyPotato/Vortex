#pragma once
#include <Graphics/Primitives/GPVertexBuffer.h>
#include <Graphics/Primitives/GPIndexBuffer.h>
#include <Math/Matrix.h>
#include <World/Components/TransformComponent.h>

namespace Vortex
{
	class World;

	struct Quad
	{
		Quad(float width = 1.f, float height = 1.f, Math::Matrix colors = { 1.f, 1.f, 1.f, 1.f });
		~Quad();

		GPVertexBuffer* vertices = nullptr;
		GPIndexBuffer* indices = nullptr;

		void SetSize(float width, float height);
		void SetColors(Math::Matrix colors);

	private:
		struct Vertex
		{
			Math::Vector position;
			Math::Vector normal;
			Math::Vector color;
		};

		Vertex* m_Vertices;
	};

	class SpriteComponent
	{
	public:
		SpriteComponent(unsigned int owner, World* world, float width = 1.f, float height = 1.f, Math::Matrix colors = {1.f, 1.f, 1.f, 1.f});
		~SpriteComponent();

		unsigned int GetOwnerID() { return m_Owner; }

		Quad& GetQuad() { return m_Quad; }
		const Math::Matrix& GetTransform() { return m_Transform.GetTransform(); }

	private:
		unsigned int m_Owner;
		World* m_World;

		TransformComponent& m_Transform;

		Quad m_Quad;
	};
}
