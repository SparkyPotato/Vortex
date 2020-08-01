#pragma once
#include <Graphics/Primitives/GraphicsPrimitive.h>

namespace Vortex
{
	/*
		Where should the constant buffer be bound?
	*/
	enum class ConstantBufferTarget
	{
		VertexShader,
		PixelShader
	};

	class GPConstantBuffer : public GraphicsPrimitive
	{
	public:
		virtual ~GPConstantBuffer() {}
		
		/*
			Create a constant buffer with `size` data.
		*/
		static GPConstantBuffer* Create(void* data, int size, ConstantBufferTarget target);

		virtual void Bind() = 0;
		virtual void Recreate() = 0;
		
		/*
			Changes the data in the buffer.
		*/
		virtual void Set(void* newData) = 0;
	};
}
