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
