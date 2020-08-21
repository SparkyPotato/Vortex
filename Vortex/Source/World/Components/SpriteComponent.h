#pragma once
#include <Graphics/Primitives/GPVertexBuffer.h>
#include <Graphics/Primitives/GPIndexBuffer.h>
#include <Graphics/Primitives/GPTexture.h>
#include <Math/Matrix.h>
#include <World/Components/TransformComponent.h>

namespace Vortex
{
	class World;

	struct Quad
	{
		Quad(float width = 10.f, float height = 10.f, Math::Matrix colors = { 1.f, 1.f, 1.f, 1.f });
		~Quad();

		GPVertexBuffer* vertices = nullptr;
		GPIndexBuffer* indices = nullptr;

		void SetSize(float width, float height);
		void SetColors(Math::Matrix colors);

		float GetWidth() { return m_Vertices[1].position.x * 2; }
		float GetHeight() { return m_Vertices[0].position.y * 2; }

		Math::Matrix GetColors();

	private:
		struct Vertex
		{
			Math::Vector position;
			Math::Vector normal;
			Math::Vector color;
			struct
			{
				float u;
				float v;
			} texcoord;
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
		GPTexture* GetTexture() { return m_Texture; }
		void SetTexture(GPTexture* texture) { m_Texture = texture; }
		const Math::Matrix GetTransform() { return m_Transform->GetTransform(); }

	private:
		unsigned int m_Owner;
		World* m_World;

		TransformComponent* m_Transform;

		Quad m_Quad;
		GPTexture* m_Texture = nullptr;
	};
}
