#pragma once
#include <Graphics/Primitives/GraphicsPrimitive.h>
#include <string>
#include <vector>

namespace Vortex
{
	enum class ShaderDataType
	{
		float1 = sizeof(float),
		float2 = sizeof(float) * 2,
		float3 = sizeof(float) * 3,
		float4 = sizeof(float) * 4,

		float2x2 = sizeof(float) * 2 * 2,
		float3x3 = sizeof(float) * 3 * 3,
		float4x4 = sizeof(float) * 4 * 4,
	};

	struct VertexElement
	{
		VertexElement(std::string Semantic, ShaderDataType type)
			: semantic(Semantic), dataType(type), offset(0)
		{}

		int GetSize() { return (int) dataType; }

		std::string semantic;
		ShaderDataType dataType;
		size_t offset;
	};

	struct VertexLayout
	{
		VertexLayout(std::initializer_list<VertexElement> elements)
			: m_Elements(elements)
		{ Calculate(); }

		int GetStride() { return m_Size; }

	private:
		void Calculate()
		{
			m_Size = 0;
			int offset = 0;
			for (auto& element : m_Elements)
			{
				m_Size += element.GetSize();
				element.offset = offset;
				offset += element.GetSize();
			}
		}

		std::vector<VertexElement> m_Elements;
		int m_Size;
	};

	class GPVertexBuffer : public GraphicsPrimitive
	{
	public:
		virtual ~GPVertexBuffer() {}

		static GPVertexBuffer* Create(void* vertices, int count, const VertexLayout& layout);

		virtual void Bind() = 0;
		virtual void Recreate() = 0;
	};
}
