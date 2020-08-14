#include <VXpch.h>
#include <World/Components/SpriteComponent.h>
#include <World/World.h>

namespace Vortex
{
	Quad::Quad(float height /*= 1.f*/, float width /*= 1.f*/)
	{
		struct Vertex
		{
			Math::Vector position;
			Math::Vector normal;
			Math::Vector color;
		};

		VertexLayout layout =
		{
			{ "POSITION", ShaderDataType::float4 },
			{ "COLOR", ShaderDataType::float4 },
			{ "NORMAL", ShaderDataType::float4 }
		};

		Vertex vertexArray[4];

		vertexArray[0].position = { -width / 2, height / 2, 0.f };
		vertexArray[1].position = { width / 2, height / 2, 0.f };
		vertexArray[2].position = { -width / 2, -height / 2, 0.f };
		vertexArray[3].position = { width / 2, -height / 2, 0.f };

		vertexArray[0].color = { 1.f, 1.f, 1.f, 1.f };
		vertexArray[1].color = { 1.f, 1.f, 1.f, 1.f };
		vertexArray[2].color = { 1.f, 1.f, 1.f, 1.f };
		vertexArray[3].color = { 1.f, 1.f, 1.f, 1.f };

		vertexArray[0].normal = { 0.f, 0.f, -1.f, 0.f };
		vertexArray[1].normal = { 0.f, 0.f, -1.f, 0.f };
		vertexArray[2].normal = { 0.f, 0.f, -1.f, 0.f };
		vertexArray[3].normal = { 0.f, 0.f, -1.f, 0.f };

		unsigned int indexArray[6] =
		{
			0, 1, 2,
			2, 1, 3
		};

		vertices = GPVertexBuffer::Create(vertexArray, 4, layout);
		indices = GPIndexBuffer::Create(indexArray, 6);
	}

	Quad::~Quad()
	{
		delete vertices;
		delete indices;
	}

	void Quad::CalculateMatrix(const Math::Vector& position, const Math::Vector& rotation, const Math::Vector& scale)
	{
		using namespace Math;

		transformation = Matrix::Rotate(rotation) * Matrix::Scale(scale) * Matrix::Translate(position);
	}

	SpriteComponent::SpriteComponent(unsigned int owner, World* world)
		: m_Owner(owner), m_World(world)
	{
		auto transform = m_World->GetEntityFromID(m_Owner)->GetTransform();
		if (!transform)
		{
			ENG_ERROR("Entity does not exist!");
			return;
		}

		m_Quad.CalculateMatrix(transform->GetPosition(), transform->GetRotation(), transform->GetScale());
	}

	SpriteComponent::~SpriteComponent()
	{

	}
}
