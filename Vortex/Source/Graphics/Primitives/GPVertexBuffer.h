#pragma once
#include <Graphics/Primitives/GraphicsPrimitive.h>

namespace Vortex
{
	/*
		Data type of the input to be passed in to the shader.
	*/
	enum class ShaderDataType
	{
		col1,
		col2,
		col4,

		float1,
		float2,
		float3,
		float4,

		float3x3,
		float4x4
	};

	/*
		An element of the vertex structure.
	*/
	struct VertexElement
	{
		VertexElement(std::string Semantic, ShaderDataType type)
			: semantic(Semantic), dataType(type), offset(0)
		{}

		int GetSize()
		{
			switch (dataType)
			{
			case ShaderDataType::col1: return 1;
			case ShaderDataType::col2: return 2;
			case ShaderDataType::col4: return 4;
			case ShaderDataType::float1: return sizeof(float);
			case ShaderDataType::float2: return sizeof(float) * 2;
			case ShaderDataType::float3: return sizeof(float) * 3;
			case ShaderDataType::float4: return sizeof(float) * 4;
			case ShaderDataType::float3x3: return sizeof(float) * 3 * 3;
			case ShaderDataType::float4x4: return sizeof(float) * 4 * 4;
			}

			return 0;
		}

		bool operator==(const VertexElement& other) const
		{
			if (semantic == other.semantic && dataType == other.dataType) return true;
			return false;
		}

		std::string semantic;
		ShaderDataType dataType;
		size_t offset;
	};

	/*
		Describes the layout of a single vertex in a buffer.
	*/
	struct VertexLayout
	{
		VertexLayout(std::initializer_list<VertexElement> elements)
			: m_Elements(elements)
		{ Calculate(); }

		int GetStride() const { return m_Size; }
		int GetElementCount() const
		{
			int i = 0;
			for (auto& element : m_Elements)
			{
				if (element.dataType != ShaderDataType::float3x3 || element.dataType != ShaderDataType::float4x4)
					i++;
				else if (element.dataType == ShaderDataType::float3x3)
					i += 3;
				else if (element.dataType == ShaderDataType::float4x4)
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

	enum class BufferAccessType
	{
		Static, Dynamic
	};

	class GPVertexBuffer : public GraphicsPrimitive
	{
	public:
		virtual ~GPVertexBuffer() {}

		/*
			Creates a vertex buffer from `count` vertices according to the the layout.
		*/
		static GPVertexBuffer* Create(void* vertices, unsigned int count, const VertexLayout& layout, BufferAccessType accessType);
		static GPVertexBuffer* Create(unsigned int size, VertexLayout& layout);

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void Recreate() = 0;
		virtual void Set(void* vertices, unsigned int count) = 0;

		virtual unsigned int GetSize() = 0;

		virtual const VertexLayout& GetLayout() = 0;
	};
}
