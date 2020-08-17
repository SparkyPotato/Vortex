#pragma once
#include <Graphics/Primitives/GPVertexBuffer.h>
#include <Graphics/Primitives/GPIndexBuffer.h>
#include <Math/Matrix.h>
#include <World/Components/TransformComponent.h>

namespace Vortex
{
	class World;

	class MeshComponent
	{
	public:
		MeshComponent(unsigned int owner, World* world);
		~MeshComponent();

		MeshComponent& operator=(const MeshComponent& other)
		{
			m_Owner = other.m_Owner;
			m_World = other.m_World;
			m_Transform = other.m_Transform;
			m_Vertices = other.m_Vertices;
			m_Indices = other.m_Indices;

			return *this;
		}

		void LoadFromFile(std::string file);

		unsigned int GetOwnerID() { return m_Owner; }

		GPVertexBuffer* GetVertexBuffer() { return m_Vertices; }
		GPIndexBuffer* GetIndexBuffer() { return m_Indices; }

		Math::Matrix GetTransform() { return m_Transform.GetTransform(); }

	private:
		unsigned int m_Owner;
		World* m_World;

		TransformComponent& m_Transform;

		GPVertexBuffer* m_Vertices  = nullptr;
		GPIndexBuffer* m_Indices = nullptr;
	};
}
