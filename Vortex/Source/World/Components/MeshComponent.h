#pragma once
#include <Graphics/Primitives/GPVertexBuffer.h>
#include <Graphics/Primitives/GPIndexBuffer.h>
#include <Math/Matrix.h>

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

		const Math::Matrix& GetTransformationMatrix() { return m_Transformation; }
		GPVertexBuffer* GetVertexBuffer() { return m_Vertices; }
		GPIndexBuffer* GetIndexBuffer() { return m_Indices; }

		void CalculateMatrix(const Math::Vector& position, const Math::Vector& rotation, const Math::Vector& scale);

	private:
		unsigned int m_Owner;
		World* m_World;

		GPVertexBuffer* m_Vertices  = nullptr;
		GPIndexBuffer* m_Indices = nullptr;

		Math::Matrix m_Transformation;
	};
}
