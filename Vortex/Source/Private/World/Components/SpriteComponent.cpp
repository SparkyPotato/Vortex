#include <VXpch.h>
#include <World/Components/SpriteComponent.h>
#include <World/World.h>

namespace Vortex
{
	Quad::Quad(float width, float height, Math::Matrix colors)
	{
		m_Vertices = new Vertex[4];

		VertexLayout layout =
		{
			{ "POSITION", ShaderDataType::float4 },
			{ "NORMAL", ShaderDataType::float4 },
			{ "COLOR", ShaderDataType::float4 }
		};

		m_Vertices[0].position = { -width / 2, height / 2, 0.f };
		m_Vertices[1].position = { width / 2, height / 2, 0.f };
		m_Vertices[2].position = { -width / 2, -height / 2, 0.f };
		m_Vertices[3].position = { width / 2, -height / 2, 0.f };

		m_Vertices[0].color = colors.columns[0];
		m_Vertices[1].color = colors.columns[1];
		m_Vertices[2].color = colors.columns[2];
		m_Vertices[3].color = colors.columns[3];

		m_Vertices[0].normal = { 0.f, 0.f, -1.f, 0.f };
		m_Vertices[1].normal = { 0.f, 0.f, -1.f, 0.f };
		m_Vertices[2].normal = { 0.f, 0.f, -1.f, 0.f };
		m_Vertices[3].normal = { 0.f, 0.f, -1.f, 0.f };

		unsigned int indexArray[6] =
		{
			0, 1, 2,
			2, 1, 3
		};

		vertices = GPVertexBuffer::Create(m_Vertices, 4, layout, BufferAccessType::Dynamic);
		indices = GPIndexBuffer::Create(indexArray, 6);
	}

	Quad::~Quad()
	{
		delete[] m_Vertices;
		delete vertices;
		delete indices;
	}

	void Quad::SetSize(float width, float height)
	{
		m_Vertices[0].position = { -width / 2, height / 2, 0.f };
		m_Vertices[1].position = { width / 2, height / 2, 0.f };
		m_Vertices[2].position = { -width / 2, -height / 2, 0.f };
		m_Vertices[3].position = { width / 2, -height / 2, 0.f };

		vertices->Set(m_Vertices, 4);
	}

	void Quad::SetColors(Math::Matrix colors)
	{
		m_Vertices[0].color = colors.columns[0];
		m_Vertices[1].color = colors.columns[1];
		m_Vertices[2].color = colors.columns[2];
		m_Vertices[3].color = colors.columns[3];

		vertices->Set(m_Vertices, 4);
	}

	Math::Matrix Quad::GetColors()
	{
		return
		{
			{ m_Vertices[0].color.x, m_Vertices[1].color.x, m_Vertices[2].color.x, m_Vertices[3].color.x },
			{ m_Vertices[0].color.y, m_Vertices[1].color.y, m_Vertices[2].color.y, m_Vertices[3].color.y },
			{ m_Vertices[0].color.z, m_Vertices[1].color.z, m_Vertices[2].color.z, m_Vertices[3].color.z },
			{ m_Vertices[0].color.w, m_Vertices[1].color.w, m_Vertices[2].color.w, m_Vertices[3].color.w }
		};
	}

	SpriteComponent::SpriteComponent(unsigned int owner, World* world, float width, float height, Math::Matrix colors)
		: m_Owner(owner), m_World(world), m_Quad(width, height, colors), m_Transform(m_World->GetEntityFromID(m_Owner)->GetTransform())
	{

	}

	SpriteComponent::~SpriteComponent()
	{

	}
}
