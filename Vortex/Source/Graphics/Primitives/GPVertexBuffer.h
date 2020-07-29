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

		float3x3 = sizeof(float) * 3 * 3,
		float4x4 = sizeof(float) * 4 * 4,
	};

	struct VertexElement
	{
		VertexElement(std::string Semantic, ShaderDataType type)
			: semantic(Semantic), dataType(type), offset(0)
		{}

		int GetSize() { return (int) dataType; }

		bool operator==(const VertexElement& other) const
		{
			if (semantic == other.semantic && dataType == other.dataType) return true;
			return false;
		}

		std::string semantic;
		ShaderDataType dataType;
		size_t offset;
	};

	struct VertexLayout
	{
		VertexLayout(std::initializer_list<VertexElement> elements)
			: m_Elements(elements)
		{ Calculate(); }

		int GetStride() const { return m_Size; }
		int GetElementCount() const
		{
			int i = 0;
			for (auto element : m_Elements)
			{
				if ((int) element.dataType < sizeof(float) * 3 * 3)
					i++;
				else if (element.dataType == ShaderDataType::float3x3)
					i += 3;
				else
					i += 4;
			}
			return i;
		}

		const std::vector<VertexElement>& GetElements() const { return m_Elements; }

		bool operator!=(const VertexLayout& other) const
		{
			if (other.m_Elements == m_Elements) return false;
			return true;
		}

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

		virtual const VertexLayout& GetLayout() = 0;
	};
}
